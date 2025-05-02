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
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i][j] = rand() % 10;  // random numbers between 0 and 99
}

// Function to display matrix
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (auto elem : row)
            cout << elem << " ";
        cout << "\n";
    }
}

// Sequential Matrix Multiplication
void sequentialMatrixMultiplication(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int rowsA = A.size(), colsA = A[0].size();
    int colsB = B[0].size();

    for (int i = 0; i < rowsA; i++)
        for (int j = 0; j < colsB; j++) {
            C[i][j] = 0;
            for (int k = 0; k < colsA; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
}

// Parallel Matrix Multiplication using OpenMP
void parallelMatrixMultiplication(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int rowsA = A.size(), colsA = A[0].size();
    int colsB = B[0].size();

    #pragma omp parallel for
    for (int i = 0; i < rowsA; i++)
        for (int j = 0; j < colsB; j++) {
            C[i][j] = 0;
            for (int k = 0; k < colsA; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
}

int main() {
    int rowsA, colsA, rowsB, colsB;

    // Input matrix dimensions
    cout << "Enter number of rows and columns for matrix A: ";
    cin >> rowsA >> colsA;
    cout << "Enter number of rows and columns for matrix B: ";
    cin >> rowsB >> colsB;

    if (colsA != rowsB) {
        cout << "Matrix multiplication not possible due to incompatible dimensions.\n";
        return 0;
    }

    // Initialize and generate matrices
    srand(time(0));
    vector<vector<int>> A(rowsA, vector<int>(colsA));
    vector<vector<int>> B(rowsB, vector<int>(colsB));
    vector<vector<int>> C(rowsA, vector<int>(colsB));         // Sequential result
    vector<vector<int>> C_parallel(rowsA, vector<int>(colsB)); // Parallel result

    generateMatrix(A, rowsA, colsA);
    generateMatrix(B, rowsB, colsB);

    // Print matrices A and B
    cout << "\nMatrix A:\n";
    printMatrix(A);
    cout << "\nMatrix B:\n";
    printMatrix(B);

    // Sequential Multiplication
    auto start_seq = high_resolution_clock::now();
    sequentialMatrixMultiplication(A, B, C);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end_seq - start_seq);

    cout << "\nResult Matrix (Sequential):\n";
    printMatrix(C);
    cout << "Sequential Time: " << duration_seq.count() << " ms\n";

    // Parallel Multiplication
    auto start_par = high_resolution_clock::now();
    parallelMatrixMultiplication(A, B, C_parallel);
    auto end_par = high_resolution_clock::now();
    auto duration_par = duration_cast<milliseconds>(end_par - start_par);

    cout << "\nResult Matrix (Parallel - OpenMP):\n";
    printMatrix(C_parallel);
    cout << "Parallel Time: " << duration_par.count() << " ms\n";

    return 0;
}
