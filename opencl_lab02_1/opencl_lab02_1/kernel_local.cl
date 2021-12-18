
__kernel void transposeMatrices_OpenCL(__global int* A, __global int* B, int M, int N) {

    __local int local_memory[16][16];

    int txLocal = get_local_id(0);
    int tyLocal = get_local_id(1);
    int txGlobal = get_global_id(0);
    int tyGlobal = get_global_id(1);

    if (tyGlobal < M && txGlobal < N) {
        local_memory[tyLocal][txLocal] = A[(tyGlobal * N) + txGlobal];
    }

    int new_txGlobal = get_group_id(0) * 16 + tyLocal;
    int new_tyGlobal = get_group_id(1) * 16 + txLocal;

    barrier(CLK_LOCAL_MEM_FENCE);

    if (new_tyGlobal < M && new_txGlobal < N) 
        B[(new_txGlobal * M + new_tyGlobal)] = local_memory[txLocal][tyLocal];
}