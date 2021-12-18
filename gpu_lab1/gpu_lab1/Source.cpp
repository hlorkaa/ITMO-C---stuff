#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <math.h>
#include <CL/cl.h>



using namespace std;


// наполн€ет матрицу случайными значени€ми
void fill_matrix(int* matrix, int size)
{
	for (int i = 0; i < size; ++i)
	{
		matrix[i] = int(rand() % 100 + 1);
	}
}

void printMatrix(const int const* matrix, const int& dimensionX, const int& dimensionY) {
	cout << " ";
	for (int i = 0; i < (dimensionX * dimensionY); i++) {
		cout << matrix[i] << " ";
		if (((i + 1) % dimensionX == 0) && (i != 0))
			cout << endl << " ";


	}
}

int main(int argc, char** argv)
{
	// параметры и путь  ернела
	int M, K, N;

	if (argc < 5)
		return 1;

	M = 16; // atoi(argv[1]);
	K = 16; // atoi(argv[2]);
	N = 16; // atoi(argv[3]);

	const string path = argv[4]; // ".\kernel.cl";
	bool check = atoi(argv[5]);

	if (M <= 0 or K <= 0 or N <= 0)
		return 1;

	// создаем матрицы
	int A_size = M * K;
	int* A = (int*)malloc(sizeof(int) * A_size);
	fill_matrix(A, A_size);

	int B_size = K * N;
	int* B = (int*)malloc(sizeof(int) * B_size);
	fill_matrix(B, B_size);

	int C_size = M * N;
	int* C = (int*)malloc(sizeof(int) * C_size);

	cl_int error; // дл€ ошибок

	// задаем платформу, устройство, контекст и очередь команд
	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_context context;
	cl_command_queue command_queue;

	error = clGetPlatformIDs(1, &platform_id, NULL);
	error = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &error);
	command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &error);

	// выдел€ем пам€ть на устройстве дл€ массивов
	//cl_mem A_buffer;
	//cl_mem B_buffer;
	//cl_mem C_buffer;

	cl_mem A_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int) * A_size, A, &error);
	cl_mem B_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int) * B_size, B, &error);
	cl_mem C_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * C_size, NULL, &error);

	// создаем программу и кернел, устанавливаем аргументы, запускаем его
	cl_program program;
	cl_kernel kernel;
	cl_event waitEvent;

	ifstream file(path);
	string code_text(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));
	char* code = new char[code_text.length() + 1];
	strcpy_s(code, code_text.size() + 1, code_text.c_str());

	program = clCreateProgramWithSource(context, 1, (const char**)&code, NULL, &error);
	error = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	kernel = clCreateKernel(program, "multiplication", &error);

	error = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&A_buffer);
	error = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&B_buffer);
	error = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&C_buffer);
	error = clSetKernelArg(kernel, 3, sizeof(cl_int), (void*)&M);
	error = clSetKernelArg(kernel, 4, sizeof(cl_int), (void*)&K);
	error = clSetKernelArg(kernel, 5, sizeof(cl_int), (void*)&N);

	size_t local_size[2] = { 16, 16 };
	size_t global_size[2] = { 16 * M, 16 * N };//{ local_size[0] * M, local_size[1] * N };

	error = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_size, local_size, 0, NULL, &waitEvent);

	// получаем результат, сравниваем с результатом от процессора
	clWaitForEvents(1, &waitEvent);
	error = clFinish(command_queue);

	clEnqueueReadBuffer(command_queue, C_buffer, CL_TRUE, 0, sizeof(int) * (M * N), C, 0, NULL, NULL);

	printMatrix(C, M, N);
	//if (check)
	//{
	int* C_onCPU = (int*)malloc(sizeof(int) * C_size);

	for (size_t row = 0; row < M; ++row) {
		for (size_t column = 0; column < N; ++column) {
			C_onCPU[row * N + column] = 0;
			for (size_t k = 0; k < K; ++k) {
				C_onCPU[row * N + column] +=
					A[row * K + k] *
					B[k * N + column];
			}
		}
	}

	bool isEqual = true;
	for (int i = 0; i < (M * N); ++i)
	{
		if (C[i] != C_onCPU[i])
			isEqual = false;
	}

	if (isEqual)
		cout << "OK" << endl;
	else
		cout << "ERROR" << endl;

	printMatrix(C_onCPU, M, N);

	//free(C_onCPU);
//}

//free(matrixA);
//free(matrixB);
//free(matrixResult);
	clReleaseMemObject(A_buffer);
	clReleaseMemObject(B_buffer);
	clReleaseMemObject(C_buffer);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	return 0;
}