#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cstdlib>
#include <cstring>
#include <numeric>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int SIZE = 2048;
const int RUNS = 5;

// Matrix-vector multiplication
void multiply_mv_row_major(const double* matrix, int rows, int cols, const double* vector, double* result) {
    memset(result, 0, sizeof(double) * rows);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            result[i] += matrix[i * cols + j] * vector[j];
    }
}

// Aligned memory allocation
double* aligned_alloc_array(size_t size, size_t alignment = 128) {
    return reinterpret_cast<double*>(std::aligned_alloc(alignment, sizeof(double) * size));
}

void aligned_free_array(double* ptr) {
    free(ptr);
}

// Benchmark helper
template<typename Func>
double benchmark(Func&& func, int runs) {
    vector<double> times;
    for(int i = 0; i < runs; ++i) {
        auto start = high_resolution_clock::now();
        func();
        auto end = high_resolution_clock::now();
        times.push_back(duration_cast<microseconds>(end-start).count());
    }
    double mean = accumulate(times.begin(), times.end(), 0.0) / runs;
    cout << "Average: " << mean << " us\n";
    return mean;
}

void print_vector(const double* v, int size) {
    for (int i = 0; i < size; ++i)
        cout << v[i] << " ";
    cout << "\n";
}

void print_matrix(const double* m, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            cout << setw(5) << m[i * cols + j] << " ";
        cout << "\n";
    }
}

int main() {
    const int total_size = SIZE * SIZE;

    // Unaligned memory
    double* matrix_unaligned = new double[total_size];
    double* vector_unaligned = new double[SIZE];
    double* result_unaligned = new double[SIZE];

    // Aligned memory (64-byte)
    double* matrix_aligned = aligned_alloc_array(total_size);
    double* vector_aligned = aligned_alloc_array(SIZE);
    double* result_aligned = aligned_alloc_array(SIZE);

    // Initialize data
    fill_n(matrix_unaligned, total_size, 1.0);
    fill_n(vector_unaligned, SIZE, 1.0);
    fill_n(matrix_aligned, total_size, 1.0);
    fill_n(vector_aligned, SIZE, 1.0);

    // Benchmark Unaligned
    auto test_unaligned = [&]() {
        multiply_mv_row_major(matrix_unaligned, SIZE, SIZE, vector_unaligned, result_unaligned);
    };

    // Benchmark Aligned
    auto test_aligned = [&]() {
        multiply_mv_row_major(matrix_aligned, SIZE, SIZE, vector_aligned, result_aligned);
    };

    cout << "Unaligned Memory:\n";
    benchmark(test_unaligned, RUNS);

    cout << "\nAligned Memory (128-byte):\n";
    benchmark(test_aligned, RUNS);

    // Free memory
    delete[] matrix_unaligned;
    delete[] vector_unaligned;
    delete[] result_unaligned;

    aligned_free_array(matrix_aligned);
    aligned_free_array(vector_aligned);
    aligned_free_array(result_aligned);

    return 0;
}
