#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

void quicksort(std::vector<int>& arr, int left, int right, int depth);
int partition(std::vector<int>& arr, int left, int right);

void insertion_sort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void quicksort(std::vector<int>& arr, int left, int right, int depth) {
    const int THRESHOLD = 10; //用于决定在快速排序算法中何时切换到插入排序。通常，当数组的大小小于或等于这个阈值时，插入排序会比快速排序更有效率。
    const int MAX_DEPTH = 4; //防止创建过多线程，限制递归深度

    if (right - left <= THRESHOLD) {
        insertion_sort(arr, left, right);
    } else if (left < right) {
        int pivot_index = partition(arr, left, right);
        
        if (depth < MAX_DEPTH) {
            // 创建左、右部分的线程，但限制递归深度
            std::thread left_thread(quicksort, std::ref(arr), left, pivot_index - 1, depth + 1);
            std::thread right_thread(quicksort, std::ref(arr), pivot_index + 1, right, depth + 1);
            
            // 启动线程并等待结束
            left_thread.join();
            right_thread.join();
        } else {
            quicksort(arr, left, pivot_index - 1, depth + 1);
            quicksort(arr, pivot_index + 1, right, depth + 1);
        }
    }
}

int partition(std::vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    int pivot = std::max(std::min(arr[left], arr[right]), std::min(std::max(arr[left], arr[right]), arr[mid]));
    if (pivot == arr[mid]) {
        std::swap(arr[mid], arr[right]);
    } else if (pivot == arr[left]) {
        std::swap(arr[left], arr[right]);
    }

    int i = left - 1;
    for (int j = left; j < right; ++j) {
        if (arr[j] <= arr[right]) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[right]);
    return i + 1;
}

void threaded_quicksort(std::vector<int>& arr) {
    quicksort(arr, 0, arr.size() - 1, 0);
}

int main() {
    std::vector<int> arr = {100, 3, 50, 200, 33, 17, 25, 400, 120, 75, 6, 8, 10, 1, 2, 1, 15, 7, 9, 250, 300, 4, 180, 170, 90, 55, 85, 95, 77, 65};
    std::cout << "Original array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    threaded_quicksort(arr);

    std::cout << "Sorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}