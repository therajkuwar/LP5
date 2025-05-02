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