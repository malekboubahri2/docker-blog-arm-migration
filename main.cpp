#include "matrix_operations.h"
#include <iostream>

int main() {
    std::cout << "ARM64 NEON Matrix Operations Benchmark" << std::endl;
    std::cout << "========================================" << std::endl;

#if defined(__aarch64__) || defined(__ARM_ARCH)
    std::cout << "Running on ARM64 architecture with NEON optimizations" << std::endl;
#elif defined(__x86_64__) || defined(_M_X64)
    std::cout << "Running on x86-64 architecture (fallback build)" << std::endl;
#else
    #error "This code requires ARM64 or x86-64 architecture"
#endif

    benchmark_matrix_ops();

    return 0;
}