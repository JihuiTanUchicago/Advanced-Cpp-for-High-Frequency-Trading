### Team Member:
Todd Tan (just myself)

### Benchmarking
compile with -O0
```
clang++ -std=c++17 -O0 high_performance_linear_algebra_kernels.cpp -o high_performance_linear_algebra_kernels_with_O0
```
```
Test 1: multiply_mv_row_major
Mean: 166.4us, Std Dev: 94.9855ns
6 15 

Test 2: multiply_mv_col_major
Mean: 166.8us, Std Dev: 105.304ns
6 15 

Test 3: multiply_mm_naive
Mean: 258.2us, Std Dev: 145.037ns
   58    64 
  139   154 

Test 4: multiply_mm_transposed_b
Mean: 250us, Std Dev: 108.449ns
   58    64 
  139   154 
```

compile with -O3
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

### Cache Locality Analysis:
- `multiply_mv_col_major` is expected to be faster than `multiply_mv_row_major` since it is accessing contiguous memory.
- `multiply_mm_transposed_b` is expected to be faster than its naive counterpart, since both matrix A and B are accessing contiguous memory.
- Both expectations are confirmed through the benmark results above.


### Unaligned vs Aligned memory 
compile:
```
clang++ -std=c++17 -O0 memory_alignment.cpp -o memory_alignment
```
```
Unaligned Memory:
Average: 7634.4 us

Aligned Memory (128-byte):
Average: 5984.2 us
```
- Aligned memory is beneficial if aligned memory is no more than how much cache lines can hold. Too much aligned memory will introduce overheads.