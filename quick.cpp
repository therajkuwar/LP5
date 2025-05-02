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


int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];           // Choose the last element as pivot
    int i = (low - 1);               // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);    // Move smaller element to correct position
        }
    }
    swap(arr[i + 1], arr[high]);     // Place pivot in correct position
    return (i + 1);                  // Return pivot index
}


// Sequential Quick Sort
void sequentialQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);  // Partitioning index

        sequentialQuickSort(arr, low, pi - 1);  // Sort left part
        sequentialQuickSort(arr, pi + 1, high);  // Sort right part
    }
}

// Parallel Quick Sort using OpenMP
void parallelQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);  // Partitioning index

        // Parallelize the recursive calls
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelQuickSort(arr, low, pi - 1);  // Sort left part

            #pragma omp section
            parallelQuickSort(arr, pi + 1, high);  // Sort right part
        }
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

    cout << "\nOriginal Array: ";
    printArray(originalArr);

    double startTime, endTime;

    // Sequential Quick Sort
    cout << "Calculating sequential time...\n";
    auto start_seq = high_resolution_clock::now();
    sequentialQuickSort(seqArr, 0, seqArr.size() - 1);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end_seq - start_seq);
    cout << "Sequential Quick Sort Time: " << duration_seq.count() << " ms\n";
    cout<<"\nSorted array using seq quick sort:"<<endl;
    for(auto i : seqArr){
        cout<<i<<" ";
    }
    cout<<endl;

    // Parallel Quick Sort
    cout << "Calculating parallel time...\n";
    auto start_par = high_resolution_clock::now();
    parallelQuickSort(parArr, 0, parArr.size() - 1);
    auto end_par = high_resolution_clock::now();
    auto duration_par = duration_cast<milliseconds>(end_par - start_par);
    cout << "Parallel Quick Sort (OpenMP) Time: " << duration_par.count() << " ms\n";
    cout<<"\nSorted array using Parallel quick sort:"<<endl;
    for(auto i : parArr){
        cout<<i<<" ";
    }
    cout<<endl;
    return 0;
}
