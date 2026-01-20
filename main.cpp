#include "matrix_operations.h"
#include <iostream>

int main() {
    std::cout << "x86-64 AVX2 Matrix Operations Benchmark" << std::endl;
    std::cout << "========================================" << std::endl;

#if defined(__x86_64__) || defined(_M_X64)
    std::cout << "Running on x86-64 architecture with AVX2 optimizations" << std::endl;
#else
    #error "This code requires x86-64 architecture with AVX2 support"
#endif

    benchmark_matrix_ops();

    return 0;
}