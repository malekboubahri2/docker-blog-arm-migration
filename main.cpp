#include "matrix_operations.h"
#include <iostream>

int main() {
    std::cout << "SIMD Matrix Operations Benchmark" << std::endl;
    std::cout << "================================" << std::endl;

#if defined(__aarch64__) || defined(_M_ARM64)
    std::cout << "Running on ARM64 architecture with NEON optimizations" << std::endl;
#elif defined(__x86_64__) || defined(_M_X64)
    std::cout << "Running on x86-64 architecture with AVX2 optimizations" << std::endl;
#else
    std::cout << "Running on generic architecture with scalar operations" << std::endl;
#endif

    benchmark_matrix_ops();

    return 0;
}