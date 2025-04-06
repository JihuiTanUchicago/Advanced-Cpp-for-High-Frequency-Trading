#include <iostream>
#include <vector>
#include <chrono>
#include <random>

#include "high_performance_linear_algebra_kernels.h"
using namespace std;
using namespace std::chrono;

const int SIZE = 4096;

// Generate random 1D vector
void generateVector(vector<int>& vector) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(-100, 100);
    for (int i = 0; i < SIZE; ++i) {
        vector[i] = distrib(gen);
    }
}
// Generate random 2D matrix
void generateMatrix(vector< vector<int> >& matrix) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(-100, 100);
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            matrix[i][j] = distrib(gen);
        }
    }
}

#include <iostream>
#include <iomanip>
#include <cstring>  // for memset
#include <cassert>

void multiply_mv_row_major(const double* matrix, int rows, int cols, const double* vector, double* result) {
    if (!matrix || !vector || !result) {
        std::cerr << "Null pointer in multiply_mv_row_major.\n";
        return;
    }

    memset(result, 0, sizeof(double) * rows);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i] += matrix[i * cols + j] * vector[j];
        }
    }
}

void multiply_mv_col_major(const double* matrix, int rows, int cols, const double* vector, double* result) {
    if (!matrix || !vector || !result) {
        std::cerr << "Null pointer in multiply_mv_col_major.\n";
        return;
    }

    memset(result, 0, sizeof(double) * rows);
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            result[i] += matrix[j * rows + i] * vector[j];
        }
    }
}

void multiply_mm_naive(const double* matrixA, int rowsA, int colsA,
                       const double* matrixB, int rowsB, int colsB,
                       double* result) {
    if (!matrixA || !matrixB || !result) {
        std::cerr << "Null pointer in multiply_mm_naive.\n";
        return;
    }
    if (colsA != rowsB) {
        std::cerr << "Incompatible dimensions in multiply_mm_naive.\n";
        return;
    }

    std::memset(result, 0, sizeof(double) * rowsA * colsB);
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i * colsB + j] += matrixA[i * colsA + k] * matrixB[k * colsB + j];
            }
        }
    }
}

void multiply_mm_transposed_b(const double* matrixA, int rowsA, int colsA,
                              const double* matrixB_transposed, int rowsB, int colsB,
                              double* result) {
    if (!matrixA || !matrixB_transposed || !result) {
        cerr << "Null pointer in multiply_mm_transposed_b.\n";
        return;
    }
    if (colsA != rowsB || rowsA != colsB) {
        cerr << "Incompatible dimensions in multiply_mm_transposed_b.\n";
        return;
    }


    memset(result, 0, sizeof(double) * rowsA * colsB);
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i * colsB + j] += matrixA[i * colsA + k] * matrixB_transposed[j * colsA + k];
            }
        }
    }
}

void print_vector(const double* v, int size) {
    for (int i = 0; i < size; ++i)
        std::cout << v[i] << " ";
    cout << "\n";
}

void print_matrix(const double* m, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            cout << setw(5) << m[i * cols + j] << " ";
        cout << "\n";
    }
}

// Benchmarking helper
template<typename Func, typename... Args>
double benchmark(Func&& func, int runs, Args&&... args) {
    vector<double> durations;
    for (int i = 0; i < runs; ++i) {
        auto start = high_resolution_clock::now();
        func(forward<Args>(args)...);
        auto end = high_resolution_clock::now();
        durations.push_back(duration_cast<nanoseconds>(end - start).count());
    }
    double mean = accumulate(durations.begin(), durations.end(), 0.0) / runs;
    double sq_sum = inner_product(durations.begin(), durations.end(), durations.begin(), 0.0);
    double stdev = sqrt(sq_sum / runs - mean * mean);
    cout << "Mean: " << mean << "us, Std Dev: " << stdev << "ns\n";
    return mean;
}

int main() {
    const int RUNS = 5;
    // Test 1: multiply_mv_row_major
    const int rows1 = 2, cols1 = 3;
    double matrix1[6] = {1, 2, 3, 4, 5, 6}; // 2x3 row-major
    double vector1[3] = {1, 1, 1};
    double* result1 = new double[rows1];

    auto test1 = [&]() {
        multiply_mv_row_major(matrix1, rows1, cols1, vector1, result1);
    };

    // Test 2: multiply_mv_col_major
    const int rows2 = 2, cols2 = 3;
    double matrix2[6] = {1, 4, 2, 5, 3, 6}; // 2x3 column-major
    double vector2[3] = {1, 1, 1};
    double* result2 = new double[rows2];
    auto test2 = [&]() {
        multiply_mv_col_major(matrix2, rows2, cols2, vector2, result2);
    };

    
    // Test 3: multiply_mm_naive
    const int rowsA = 2, colsA = 3, rowsB = 3, colsB = 2;
    double matrixA[6] = {1, 2, 3, 4, 5, 6}; // 2x3
    double matrixB[6] = {7, 8, 9, 10, 11, 12}; // 3x2
    double* result3 = new double[rowsA * colsB];
    auto test3 = [&]() {
        multiply_mm_naive(matrixA, rowsA, colsA, matrixB, rowsB, colsB, result3);
    };

    // Test 4: multiply_mm_transposed_b
    double matrixB_T[6] = {7, 9, 11, 8, 10, 12}; // 2x3 transpose of matrixB
    double* result4 = new double[rowsA * colsB];
    auto test4 = [&]() {
        multiply_mm_transposed_b(matrixA, rowsA, colsA, matrixB_T, rowsB, colsB, result4);
    };
    

    cout << "Test 1: multiply_mv_row_major\n";
    benchmark(test1, RUNS);
    print_vector(result1, rows1);  // Expected output: 6 15
    delete[] result1;

    cout << "\nTest 2: multiply_mv_col_major\n";
    benchmark(test2, RUNS);
    print_vector(result2, rows2);  // Expected: [6, 15]
    delete[] result2;

    cout << "\nTest 3: multiply_mm_naive\n";
    benchmark(test3, RUNS);
    print_matrix(result3, rowsA, colsB);  // Expected: [[58, 64], [139, 154]]
    delete[] result3;

    cout << "\nTest 4: multiply_mm_transposed_b\n";
    benchmark(test4, RUNS);
    print_matrix(result4, rowsA, colsB);  // Expected: [[58, 64], [139, 154]]
    delete[] result4;

    return 0;
}
