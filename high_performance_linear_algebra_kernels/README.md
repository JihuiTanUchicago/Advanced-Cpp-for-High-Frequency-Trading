### Output
compile
```
clang++ -std=c++17 -O3 high_performance_linear_algebra_kernels.cpp -o high_performance_linear_algebra_kernels
```
```
Test 1: multiply_mv_row_major
Mean: 50.2us, Std Dev: 16.9044ns
6 15 

Test 2: multiply_mv_col_major
Mean: 41.4us, Std Dev: 0.489898ns
6 15 

Test 3: multiply_mm_naive
Mean: 33.4us, Std Dev: 16.7045ns
   58    64 
  139   154 

Test 4: multiply_mm_transposed_b
Mean: 41.8us, Std Dev: 0.4ns
   58    64 
  139   154 
```