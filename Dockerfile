FROM ubuntu:22.04

# Install build tools for ARM64 and x86-64 cross-compilation support
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY *.h *.cpp ./

# Build with architecture-specific optimizations
# ARM64: NEON intrinsics enabled by default, use -march for specific CPU targets
# x86-64: Use -mavx2 for AVX2 support
ARG TARGETARCH
RUN if [ "$TARGETARCH" = "arm64" ]; then \
        g++ -O2 -march=armv8-a+simd -o benchmark \
        main.cpp \
        matrix_operations.cpp \
        -std=c++11; \
    else \
        g++ -O2 -mavx2 -o benchmark \
        main.cpp \
        matrix_operations.cpp \
        -std=c++11; \
    fi

CMD ["./benchmark"]