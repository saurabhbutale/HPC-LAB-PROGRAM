#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// Parallel Bubble Sort using OpenMP (Odd-Even Transposition Sort)
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for
        for (int j = i % 2; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// Merge two sorted halves
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size())
        arr[k++] = (left[i] < right[j]) ? left[i++] : right[j++];
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

// Utility to print the array
void printArray(const vector<int>& arr, const string& msg) {
    cout << msg;
    for (int x : arr) cout << x << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    vector<int> arr(n), arr2(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];
    arr2 = arr; // duplicate for merge sort

    double start, end;

    start = omp_get_wtime();
    parallelBubbleSort(arr);
    end = omp_get_wtime();
    printArray(arr, "Sorted array using Parallel Bubble Sort: ");
    cout << "Time taken by Parallel Bubble Sort: " << end - start << " seconds\n\n";

    start = omp_get_wtime();
    parallelMergeSort(arr2, 0, n - 1);
    end = omp_get_wtime();
    printArray(arr2, "Sorted array using Parallel Merge Sort: ");
    cout << "Time taken by Parallel Merge Sort: " << end - start << " seconds\n";

    return 0;
}
/*
saurabh@Ubuntu:~$ g++ -fopenmp L2.cpp -o L2
saurabh@Ubuntu:~$ ./L2
Enter number of elements: 6
Enter elements:
5 3 2 6 1 4
Sorted array using Parallel Bubble Sort: 1 2 3 4 5 6 
Time taken by Parallel Bubble Sort: 0.0170113 seconds

Sorted array using Parallel Merge Sort: 1 2 3 4 5 6 
Time taken by Parallel Merge Sort: 0.000113751 seconds

*/
