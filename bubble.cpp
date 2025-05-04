// Implement Sequential and Parallel Bubble Sort using OpenMP and compare performance
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Function to generate a random array
void generateArray(vector<int>& arr, int size) {
    srand(time(0)); // seed the random number generator
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 10000; // random numbers between 0 and 99999
    }
}

// Function to display array
void printArray(vector<int>& arr) {
    for (auto i: arr) {
        cout << i << " ";
    }
    cout << "\n";
}

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        // cout << "sequential bubble sort\n";
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j+1]) {
                // cout << "S swap\n";
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel Bubble Sort using OpenMP (Odd-Even Transposition Sort)
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        // Even Phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Odd Phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


void copyArray(vector<int>& source, vector<int>& dest) {
    for (int i = 0; i < source.size(); i++) {
        dest[i] = source[i];
    }
}

int main(int argc, char const *argv[]) {
    int size;
    cout << "Enter size of array: ";
    cin >> size;

    vector<int> originalArr(size), seqArr(size), parArr(size);

    generateArray(originalArr, size);
    copyArray(originalArr, seqArr);
    copyArray(originalArr, parArr);

    cout << "\nOriginal Array: ";
    printArray(originalArr);

    double startTime, endTime;

    // Sequential Bubble Sort
    cout << "Calculating sequential time...\n";
    auto start_seq = high_resolution_clock::now();
    sequentialBubbleSort(seqArr);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end_seq - start_seq);
    cout << "Sequential Bubble Sort Time: " << duration_seq.count() << " ms\n";
    cout << "Sorted Array using Sequential Bubble Sort: ";
    for (int val : seqArr) {
        cout << val << " ";
    }
    cout << endl;


    // Parallel Bubble Sort
    cout << "Calculating parallel time...\n";
    auto start_par = high_resolution_clock::now();
    parallelBubbleSort(parArr);
    auto end_par = high_resolution_clock::now();
    auto duration_par = duration_cast<milliseconds>(end_par - start_par);
    cout << "Parallel Bubble Sort (OpenMP) Time: " << duration_par.count() << " ms\n";
    cout << "Sorted Array using Parallel Bubble Sort: ";
    for (int val : parArr) {
        cout << val << " ";
    }
    cout << endl;
    return 0;
}



/*
Why Parallel Bubble Sort is Slower
1. Overhead of Thread Management (OpenMP)
OpenMP creates and manages threads during parallel regions.

For small to moderate input sizes (up to even 50,000), the overhead of thread creation, synchronization, and context switching outweighs the benefits of parallelism.

2. High Synchronization in Odd-Even Sort
Each phase (odd and even) depends on the completion of the previous.

This creates a synchronization bottleneck — threads must wait between phases.

3. Memory Bandwidth Contention
Multiple threads accessing and writing to adjacent memory can lead to cache line conflicts and false sharing.

Bubble sort swaps adjacent elements — this causes frequent write operations close in memory, limiting parallel benefit.

4. Inefficiency of Bubble Sort
Bubble Sort (and its parallel version) has O(n²) time complexity regardless.

Even with parallelization, it performs too many unnecessary comparisons and swaps.

Faster algorithms (like Merge Sort or Quicksort) show much better speed-up when parallelized.

5. OpenMP Thread Scheduling and CPU Limits
If your system has 4 or 8 cores, parallelism is capped at that limit.

Beyond a certain point, you gain no performance boost because all CPU cores are already saturated.

*/
