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
        arr[i] = rand() % 10000; // random numbers between 0 and 9999
    }
}

// Function to display array
void printArray(const vector<int>& arr) {
    for (auto i: arr) {
        cout << i << " ";
    }
    cout << "\n";
}

// Merge function for Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temp arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        sequentialMergeSort(arr, left, mid);   // Sort first half
        sequentialMergeSort(arr, mid + 1, right);  // Sort second half

        merge(arr, left, mid, right);  // Merge the sorted halves
    }
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Parallelize the recursive calls
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);   // Sort first half
            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);  // Sort second half
        }

        merge(arr, left, mid, right);  // Merge the sorted halves
    }
}

void copyArray(const vector<int>& source, vector<int>& dest) {
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

    // cout << "\nOriginal Array: ";
    // printArray(originalArr);

    double startTime, endTime;

    // Sequential Merge Sort
    cout << "Calculating sequential time...\n";
    auto start_seq = high_resolution_clock::now();
    sequentialMergeSort(seqArr, 0, seqArr.size() - 1);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end_seq - start_seq);
    cout << "Sequential Merge Sort Time: " << duration_seq.count() << " ms\n";
    // cout << "\n Sorted Array (Sequential): ";
    // printArray(seqArr);



    // Parallel Merge Sort
    cout << "Calculating parallel time...\n";
    auto start_par = high_resolution_clock::now();
    parallelMergeSort(parArr, 0, parArr.size() - 1);
    auto end_par = high_resolution_clock::now();
    auto duration_par = duration_cast<milliseconds>(end_par - start_par);
    cout << "Parallel Merge Sort (OpenMP) Time: " << duration_par.count() << " ms\n";
    // cout << "\n Sorted Array (Parallel): ";
    // printArray(parArr);

    return 0;
}
