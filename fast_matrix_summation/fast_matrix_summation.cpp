#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <future>

const int SIZE = 4096;

// Basic function to access matrix elements
int getElement(const std::vector< std::vector<int> >& matrix, int row, int col) {
    return matrix[row][col];
}

// Basic function to add two integers
int add(int a, int b) {
    return a + b;
}

// Unoptimized summation function
long long sumMatrixBasic(const std::vector< std::vector<int> >& matrix) {
    long long sum = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sum = add(sum, getElement(matrix, i, j));
        }
    }
    return sum;
}


// ************** Better summation implementation ***********
long long sumMatrixOptimized(const std::vector< std::vector<int> >& matrix) {
    long long sum = 0;
    for (int i = 0; i < SIZE; ++i) {
        const std::vector<int>& rowVec = matrix[i];
        for (int j = 0; j < SIZE; ++j) {
            sum += rowVec[j];
        }
    }
    return sum;

}

int main() {
    // Generate a large random matrix
    std::vector< std::vector<int> > matrix(SIZE, std::vector<int>(SIZE));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(-100, 100);
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            matrix[i][j] = distrib(gen);
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    long long sum = sumMatrixBasic(matrix);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "Basic Sum: " << sum << std::endl;
    std::cout << "Basic Time: " << duration.count() << " nanoseconds" << std::endl;

    // optimized version
    auto start_optimized = std::chrono::high_resolution_clock::now();
    long long optimized_sum = sumMatrixOptimized(matrix);
    auto end_optimized = std::chrono::high_resolution_clock::now();
    auto duration_optimized = std::chrono::duration_cast<std::chrono::nanoseconds>(end_optimized - start_optimized);

    std::cout << "Optimized Sum: " << optimized_sum << std::endl;
    std::cout << "Optimized Time: " << duration_optimized.count() << " nanoseconds" << std::endl;

    return 0;
}