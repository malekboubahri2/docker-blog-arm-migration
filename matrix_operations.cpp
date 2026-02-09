#include "matrix_operations.h"
#include <iostream>
#include <random>
#include <chrono>
#include <stdexcept>
#include <arm_neon.h>  // ARM NEON intrinsics

Matrix::Matrix(size_t r, size_t c) : rows(r), cols(c) {
    data.resize(rows, std::vector<double>(cols, 0.0));
}

void Matrix::randomize() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10.0);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            data[i][j] = dis(gen);
        }
    }
}

Matrix Matrix::multiply(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::runtime_error("Invalid matrix dimensions for multiplication");
    }

    Matrix result(rows, other.cols);

    // ARM64 optimized using NEON for double-precision (2 elements at a time)
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < other.cols; j++) {
            float64x2_t sum_vec = vdupq_n_f64(0.0);
            size_t k = 0;

            // Process 2 elements at a time with NEON (64-bit doubles)
            for (; k + 1 < cols; k += 2) {
                // Load 2 doubles from matrix a
                float64x2_t a_vec = vld1q_f64(&data[i][k]);
                
                // Load corresponding elements from matrix b (column access)
                float64_t b_vals[2] = {other.data[k][j], other.data[k+1][j]};
                float64x2_t b_vec = vld1q_f64(b_vals);
                
                // Multiply and accumulate: sum_vec += a_vec * b_vec
                sum_vec = vaddq_f64(sum_vec, vmulq_f64(a_vec, b_vec));
            }

            // Extract results from the vector and sum them
            double sum_arr[2];
            vst1q_f64(sum_arr, sum_vec);
            double sum = sum_arr[0] + sum_arr[1];

            // Handle remaining element (if cols is odd)
            if (k < cols) {
                sum += data[i][k] * other.data[k][j];
            }

            result.data[i][j] = sum;
        }
    }

    return result;
}

double Matrix::sum() const {
    double total = 0.0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            total += data[i][j];
        }
    }
    return total;
}

void benchmark_matrix_ops() {
    std::cout << "\n=== Matrix Multiplication Benchmark ===" << std::endl;

    const size_t size = 200;
    Matrix a(size, size);
    Matrix b(size, size);

    a.randomize();
    b.randomize();

    auto start = std::chrono::high_resolution_clock::now();
    Matrix c = a.multiply(b);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Matrix size: " << size << "x" << size << std::endl;
    std::cout << "Time: " << duration.count() << " ms" << std::endl;
    std::cout << "Result sum: " << c.sum() << std::endl;
}