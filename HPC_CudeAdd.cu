#include <stdio.h>
#include <cuda_runtime.h>

// CUDA kernel to add two vectors [cite: 581, 582]
__global__ void vectorAdd(float *a, float *b, float *c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x; // Global thread index 
    if (i < n) {
        c[i] = a[i] + b[i]; // Parallel addition [cite: 587]
    }
}

// Macro to check for CUDA errors [cite: 590, 591]
#define CHECK_CUDA_ERROR(call) \
do { \
    cudaError_t err = call; \
    if (err != cudaSuccess) { \
        fprintf(stderr, "CUDA error in %s at line %d: %s\n", __FILE__, __LINE__, cudaGetErrorString(err)); \
        return 1; \
    } \
} while(0)

int main() {
    int n = 1 << 20; // Size of vectors (2^20 elements) [cite: 598]
    size_t size = n * sizeof(float); // Size in bytes [cite: 599]

    // Host memory allocation [cite: 600, 601]
    float *h_a = (float*)malloc(size);
    float *h_b = (float*)malloc(size);
    float *h_c = (float*)malloc(size);

    // Initialize input vectors [cite: 606, 607]
    for (int i = 0; i < n; i++) {
        h_a[i] = (float)i;     // matches h_a[i] float(i) logic
        h_b[i] = (float)(i * 2); // matches h_b[i] float(i*2) logic [cite: 610]
    }

    // Device memory allocation [cite: 611, 612]
    float *d_a, *d_b, *d_c;
    CHECK_CUDA_ERROR(cudaMalloc((void**)&d_a, size)); // [cite: 613]
    CHECK_CUDA_ERROR(cudaMalloc((void**)&d_b, size)); // [cite: 616]
    CHECK_CUDA_ERROR(cudaMalloc((void**)&d_c, size)); // [cite: 616]

    // Copy data to device [cite: 619, 620]
    CHECK_CUDA_ERROR(cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice));

    // Launch kernel [cite: 621]
    int threadsPerBlock = 256; // [cite: 622]
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock; // [cite: 623]
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, n);

    // Check for any kernel launch errors [cite: 624, 625]
    CHECK_CUDA_ERROR(cudaGetLastError());

    // Copy result back to host [cite: 626, 627]
    CHECK_CUDA_ERROR(cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost));

    // Print results to verify [cite: 628, 629]
    for (int i = 0; i < 9; i++) {
        printf("c[%d] = %f\n", i, h_c[i]); // [cite: 630]
    }

    // Free memory [cite: 632, 633]
    cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
    free(h_a); free(h_b); free(h_c); // [cite: 636-638]

    return 0;
}


// !nvcc -arch=sm_75 vector_add.cu -o vector_add   
// !./vector_add