#include <iostream>
#include <limits>
#include <vector>
using namespace std;
template <typename Comparable>
class AVLTree
{
public:
AVLTree();
~AVLTree();
void makeEmpty();
const Comparable & findMin() const;
const Comparable & findMax() const;
bool contains(const Comparable & x) const;
void insert(const Comparable & x);
void remove(const Comparable & x);
int treeSize() const;
int computeHeight() const;
int readRootHeight() const;
bool isBalanced() const;
bool isBST() const;
double averageDepth() const;
void removeByRank(int rank);
// the next line follows textbook Figure 4.42, Line 19
static const int ALLOWED_IMBALANCE = 1;
private:
struct AVLNode // refer to textbook, Figure 4.40
{
Comparable element;
AVLNode *left;
AVLNode *right;
int height;
AVLNode( const Comparable & theElement, AVLNode *lt, AVLNode *rt ):
element(theElement), left(lt), right(rt) {}
AVLNode( Comparable && theElement, AVLNode *lt, AVLNode *rt ):
element(move(theElement)), left(lt), right(rt) {}
};
AVLNode *root;
AVLNode * findMin( AVLNode * t ) const;
AVLNode * findMax( AVLNode * t ) const;
void makeEmpty( AVLNode * & t );
void balance(AVLNode * & t);
void rotateWithLeftChild( AVLNode * & t );
void rotateWithRightChild( AVLNode * & t );
void doubleWithLeftChild( AVLNode * & t);
void doubleWithRightChild( AVLNode * & t);
};
// constructor
template <class Comparable>
AVLTree<Comparable>::AVLTree() : root(NULL) {}
// destructor
template <class Comparable>
AVLTree<Comparable>::~AVLTree()
{
makeEmpty();
}
// public makeEmpty: follow the makeEmpty in BST, referring to textbook, Figure
4.27
template <typename Comparable>
void AVLTree<Comparable>::makeEmpty() {
makeEmpty(root);
}
// private recursive makeEmpty: follow the makeEmpty in BST, referring to textbook,
Figure 4.27
template <typename Comparable>
void AVLTree<Comparable>::makeEmpty(AVLNode * & t) {
if ( t != NULL ) {
makeEmpty(t->left);
makeEmpty(t->right);
delete t;
t = NULL;
}
}
// public findMin: follow the findMin in BST, referring to textbook, Figure 4.20
template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMin() const {
if (root == NULL) {
throw underflow_error("Tree is empty");
}
return findMin(root)->element;
}
// private findMin: follow the findMin in BST, referring to textbook, Figure 4.20
template <typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMin(AVLNode * t)
const {
if ( t == NULL ) {
return NULL;
} else if (t->left == NULL) {
return t;
} else {
return findMin(t->left);
}
}
// public findMax: follow the findMax in BST, referring to textbook, Figure 4.21
template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMax() const {
if (root == NULL) {
throw underflow_error("Tree is empty");
}
return findMax(root)->element;
}
// private findMax: follow the findMax in BST, referring to textbook, Figure 4.21
template <typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMax(AVLNode * t)
const {
if ( t == NULL ) {
return NULL;
} else if (t->right == NULL) {
return t;
} else {
return findMax(t->right);
}
}
// start our implementation:
// public contains: follow the contains in BST, referring to textbook, Figure 4.17
and Figure 4.18
template<typename Comparable>
bool AVLTree<Comparable>::contains( const Comparable & x, AVLNode * t ) const {
if(t==nullptr){
return false;
}else if(x < t->element){
return contains (x, t->left);
}else if(t->element < x){
return contains (x, t->right);
}else{
return true;
}
return false;
}
// public insert: following BST, referring to textbook, Figure 4.17 and Figure 4.23
template<typename Comparable>
void AVLTree<Comparable>::insert(const Comparable & x,AVLNode * &t) {
if(t==nullptr){
t= newAVLNode{x,nullptr,nullptr};
}else if(x<t->element){
insert(x,t->left);
}else if(t->element < x){
insert (x, t->right);
balance (t);
}
}
// public remove: refer to textbook, Figure 4.17 and Figure 4.26
template<typename Comparable>
void AVLTree<Comparable>::remove( const Comparable & x,AVLNode* &t ) {
if(t==nullptr){
return;}
if(x<t->element){
remove(x,t->left);
}else if(t->element<x){
remove (x,t->right);
}else if(t->left !=nullptr && t->right !=nullptr){
t->element = findMin(t->right)->element;
remove(t->element, t->right);
}else{
AVLNode*oldNode=t;
if(t->left !=nullptr){
t=t->left;
}else{
t=t->right;
}
delete oldNode;}
balance (t);
}

// private balance: refer to textbook, Figure 4.42, Line 21 - 40
// assume t is the node that violates the AVL condition, and we then identify which
case to use (out of 4 cases)
template<typename Comparable>
void AVLTree<Comparable>::balance(AVLNode * & t) {
if(t==nullptr){
return;
}
if(height (t->left)- height (t->right)>ALLOWED_IMBALANCE)
if(height(t->left->left) >=height (t->left->right)){
rotateWithLeftChild(t);
}else{
doubleWithLeftChild(t);
}else{
if(height(t->right)-height(t->left)>ALLOWED_IMBALANCE){
if(height(t->right->right)>=height(t->right->left)){
rotateWithRightChild(t);
}
else{
doubleWithRightChild(t);
}
t->height=max(height(t->left), height (t->right))+1;
}
}
// private rotateWithLeftChild: for case 1, referring to textbook, Figure 4.44
(code) and Figure 4.43 (visualization)
template<typename Comparable>
void AVLTree<Comparable>::rotateWithLeftChild(AVLNode * & k2) {
AvlNode*k1=k2->left;
k2->left=k1->right;
k1->right=k2;
k2->height=max(height(k2->left),height(k2->right))+1;
k1->height=max(height(k1->left),k2->height)+1;
k2=k1;
}

// private rotateWithRightChild: for case 4 (the mirrored case of case 1)
template<typename Comparable>
void AVLTree<Comparable>::rotateWithRightChild(AVLNode * & k2) {
AvlNode*k1=k2->right;
k2->right=k1->left;
k1->left=k2;
k2->height=max(height(k2->left),height(k2->right))+1;
k1->height=max(height(k1->right),k2->height)+1;
k2=k1;
}

// private doubleWithLeftChild: for case 2, see textbook, Figure 4.46 (code) and
Figure 4.45 (visualization)
template<typename Comparable>
void AVLTree<Comparable>::doubleWithLeftChild(AVLNode * & k3) {
rotateWithRightChild(k3->left);
rotateWithLeftChild(k3);
}
// private doubleWithRightChild: for case 3 (the mirrored case of case 2)
template<typename Comparable>
void AVLTree<Comparable>::doubleWithRightChild(AVLNode * & k3) {
rotateWithLeftChild(k3->right);
rotateWithLeftChild(k3);
}
// public isBalanced
template <class Comparable>
bool AVLTree<Comparable>::isBalanced(AVLNode* t) const {
if(t==nullptr){
return true;
}
int left_H= height(t->left);
int right_H= height(t-> right);
if(left_H- right_H) >1){
return false;
}
return isBalanced(t->left)&& isBalanced(t->right);
}

// public isBST
template <class Comparable>
bool AVLTree<Comparable>::isBST(AVLNode* t) const {
if(t==nullptr){
return true;
}
if((t->left !=nullptr&&findMax(t->left)->element >= t->element) || (t->right !=nullptr & findMin(t->right)->element <= t->element)){
return false;
}
return isBST(t->left)&& isBST(t->right);
}

// public treeSize
template <typename Comparable>
int AVLTree<Comparable>::treeSize(AVLNode* t) const {
if(t==nullptr){
return 0;
}
return 1+ treeSize(t->left)+treeSize(t->right);
}
// public computeHeight. See Figure 4.61 in Textbook
template <typename Comparable>
int AVLTree<Comparable>::computeHeight(AVLNode *t) const {
if(t==nullptr){
return -1;
}
return 1+ max(height(t->left),height(t->right);
}
// public readRootHeight
template <typename Comparable>
int AVLTree<Comparable>::readRootHeight() const {
if(root==nullptr){
return -1;
}
return root->height;
}
// public averageDepth
template <typename Comparable>
double AVLTree<Comparable>::averageDepth(AVLNode* t, int currentDepth, int&totalDepth, int&totalNodes) const {
if(t==nullptr){
return 0.0;
}
totalDepth+= currentDepth;
totalNodes++;
averageDepth(t->left, currentDepth+1, totalDepth, totalNodes);
averageDepth(t->right, currentDepth +1, totalDepth, totalNodes);

return 0.0;
}


// public removeByRank
template <typename Comparable>
void AVLTree<Comparable>::removeByRank(int rank,AVLNode* t, int& currentRank) {
if (t==nullptr){
return t;
}
t->left= removeByRank(rank,t->left, currentRank);
if(currentRank==rank){
t=remove(t->element, t);
return t;
}
currentRank++;
t->right=removeByRank(rank,t->right, currentRank);
return t;
}




