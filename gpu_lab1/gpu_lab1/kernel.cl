
__kernel void multiplication(__global int* A, __global int* B, __global int* C, int M, int K, int N) {
   int ty = get_global_id(0);
   int tx = get_global_id(1);
   if (ty >= M || tx >= N) {
    return;
   }

   int value = 0;
   for (int k = 0; k < K; ++k)
   {
      int elementA = A[ty * K + k];
      int elementB = B[k * N + tx];
      value += elementA * elementB;
   }

   C[ty * N + tx] = value;
}