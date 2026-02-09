FROM ubuntu:22.04

# Install build tools for ARM64
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    gcc \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY *.h *.cpp ./

# ARM64 NEON intrinsics optimizations
RUN g++ -O2 -march=armv8-a+simd -o benchmark \
    main.cpp \
    matrix_operations.cpp \
    -std=c++11

CMD ["./benchmark"]