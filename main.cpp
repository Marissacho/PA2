#include <iostream>
#include <vector>
#include <chrono>
#include <random>
class SortingComparison {
public:
	// insrtion sort
	template<typename Comparable>
	void insertionSort(std::vector<Comparable>& arr) {
		for (int p = 1; p < arr.size(); ++p) {
			Comparable tmp = std::move(arr[p]);
			int j;
			for (j = p; j > 0 && tmp < arr[j - 1]; --j) {
				arr[j] = std::move(arr[j - 1]);
			}
			arr[j] = std::move(tmp); 
		}
	}
	// quick sort
	template <typename Comparable>
	const Comparable& median3(std::vector<Comparable>& arr, int left, int right) {
		int center = (left + right) / 2;
		if (arr[center] < arr[left]) {
			std::swap(arr[left], arr[center]);
		}
		if (arr[right] < arr[left]) {
			std::swap(arr[left], arr[right]);
		}
		if (arr[right] < arr[center]) {
			std::swap(arr[center], arr[right]);
		}

		std::swap(arr[center], arr[right - 1]);
		return arr[right - 1];
	}
		template<typename Comparable>
	void quickSort(std::vector<Comparable>& arr, int left, int right) { 
		if (left + 10 <= right) {
			const Comparable& pivot = median3(arr, left, right); 
			int i = left, j = right - 1;
			for (;;) {
				while (arr[++i]<pivot){}
				while (pivot<arr[--j]){}
				if (i < j) {
					std::swap(arr[i], arr[j]);
				}
				else {
					break;
				}
			}
			std::swap(arr[i], arr[right - 1]);
			quickSort(arr, left, i - 1);
			quickSort(arr, i + 1, right);
		}
		else {
			insertionSort(arr); 
		}
	}
	// merge sort
	template <typename Comparable>
	void mergeSort(std::vector <Comparable>& arr) { 
		std::vector<Comparable> tempArr(arr.size()); 
		mergeSort(arr, tempArr, 0, arr.size() - 1); 
	}

	template <typename Comparable>
	void mergeSort(std::vector<Comparable>& arr, std::vector <Comparable>& tempArr, int left, int right) {
		if (left < right) {
			int center = (left + right) / 2;
			mergeSort(arr, tempArr, left, center);
			mergeSort(arr, tempArr, center + 1, right);
			merge(arr, tempArr, left, center + 1, right);
		}
	}
	template <typename Comparable>
	void merge(std::vector<Comparable>& arr, std::vector<Comparable>&temparr,int left, int right, int rightEnd) { 
		int leftEnd = right - 1;
		int tmpPos = left;
		int numElements = rightEnd - left + 1; 
		while (left <= leftEnd && right <= rightEnd) {
			if (arr[left] <= arr[right]) {
				temparr[tmpPos++] = std::move(arr[left++]);
			}
			else {
				temparr[tmpPos++] = std::move(arr[right++]);
			}
		}
		while (left <= leftEnd) {
			temparr[tmpPos++] = std::move(arr[left++]);    
		}
		while (right <= rightEnd) {
			temparr[tmpPos++] = std::move(arr[right++]);
		}
		for (int i = 0; i < numElements; ++i, --rightEnd) {
			arr[rightEnd] = std::move(temparr[rightEnd]);
		}
	}
	
	// generate random integers
	std::vector<int> generateRandomArray(int size) {
		std::vector<int> arr(size);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(1, 100000);
		for (int& x : arr) {
			x = dist(gen);
		}
		return arr;
	}
	// test sorted
	bool isSorted(const std::vector<int>& arr) {
		for (size_t i = 1; i < arr.size(); ++i) {
			if (arr[i] < arr[i - 1]) {
				return false;
			}
		}
		return true;
	}
	// comparison run time for three sorting algorithms
	void compareSortingAlgorithms() {
		std::vector<int> sizes = { 1000, 10000, 100000 }; // data scales
		for (int size : sizes) {
			std::vector<int> arr = generateRandomArray(size);
			std::cout << "Data size: " << size << "\n";
			// test insertion sort
			std::vector<int> arr_copy = arr;
			auto start = std::chrono::high_resolution_clock::now();
			insertionSort(arr_copy);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = end - start;
			std::cout << "Insertion Sort: " << elapsed.count() << " seconds\n";
			std::cout << "Sorted: " << (isSorted(arr_copy) ? "Yes" : "No") << "\n";
			// test quick sort
			arr_copy = arr;
			start = std::chrono::high_resolution_clock::now();
			quickSort(arr_copy, 0, arr_copy.size() - 1);
			end = std::chrono::high_resolution_clock::now();
			elapsed = end - start;
			std::cout << "Quick Sort: " << elapsed.count() << " seconds\n";
			std::cout << "Sorted: " << (isSorted(arr_copy) ? "Yes" : "No") << "\n";
			// test merge sort
			arr_copy = arr;
			start = std::chrono::high_resolution_clock::now();
			std::vector<int> temparr(arr_copy.size());
			mergeSort(arr_copy,temparr, 0, arr_copy.size() - 1);  
			end = std::chrono::high_resolution_clock::now();
			elapsed = end - start;
			std::cout << "Merge Sort: " << elapsed.count() << " seconds\n";
			std::cout << "Sorted: " << (isSorted(arr_copy) ? "Yes" : "No") << "\n";
		}
	}
private:
	// Some internal helper functions
};
int main() {
	SortingComparison sorter;
	sorter.compareSortingAlgorithms();
	return 0;
}
