
__kernel void transposeMatrices_OpenCL(__global int* A, __global int* B, int M, int N) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    if (x >= N || y >= M) {
        return;
    }

    B[y * M + x] = A[x * N + y];
}