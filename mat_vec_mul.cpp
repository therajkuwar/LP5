#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Function to generate a random matrix
void generateMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    srand(time(0));  // seed the random number generator
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i][j] = rand() % 10;  // random numbers between 0 and 9
}

// Function to generate a random vector
void generateVector(vector<int>& vec, int size) {
    for (int i = 0; i < size; i++)
        vec[i] = rand() % 10;  // random numbers between 0 and 9
}

// Function to display a matrix
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (auto val : row)
            cout << val << " ";
        cout << "\n";
    }
}

// Function to display a vector
void printVector(const vector<int>& vec) {
    for (auto val : vec)
        cout << val << " ";
    cout << "\n";
}

// Sequential matrix-vector multiplication
void sequentialMatrixVectorMultiplication(const vector<vector<int>>& A, const vector<int>& x, vector<int>& result) {
    int rows = A.size(), cols = A[0].size();
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++)
            result[i] += A[i][j] * x[j];
    }
}

// Parallel matrix-vector multiplication using OpenMP
void parallelMatrixVectorMultiplication(const vector<vector<int>>& A, const vector<int>& x, vector<int>& result) {
    int rows = A.size(), cols = A[0].size();
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++)
            result[i] += A[i][j] * x[j];
    }
}

int main() {
    int rows, cols;
    cout << "Enter number of rows and columns for matrix A: ";
    cin >> rows >> cols;

    vector<vector<int>> A(rows, vector<int>(cols));
    vector<int> x(cols), result_seq(rows), result_par(rows);

    generateMatrix(A, rows, cols);
    generateVector(x, cols);

    // Print input matrix and vector
    cout << "\nMatrix A:\n";
    printMatrix(A);

    cout << "\nVector x:\n";
    printVector(x);

    // Sequential multiplication
    auto start_seq = high_resolution_clock::now();
    sequentialMatrixVectorMultiplication(A, x, result_seq);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end_seq - start_seq);

    cout << "\nSequential Result Vector:\n";
    printVector(result_seq);
    cout << "Sequential Time: " << duration_seq.count() << " ms\n";

    // Parallel multiplication
    auto start_par = high_resolution_clock::now();
    parallelMatrixVectorMultiplication(A, x, result_par);
    auto end_par = high_resolution_clock::now();
    auto duration_par = duration_cast<milliseconds>(end_par - start_par);

    cout << "\nParallel Result Vector:\n";
    printVector(result_par);
    cout << "Parallel Time (OpenMP): " << duration_par.count() << " ms\n";

    return 0;
}
