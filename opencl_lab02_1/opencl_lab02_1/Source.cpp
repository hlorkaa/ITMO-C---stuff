
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <math.h>
#include <CL/cl.h>

using namespace std;
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define KERNEL_NAME "transposeMatrices_OpenCL"

void printMatrix(const int const* matrix, const int& dimensionX, const int& dimensionY) {
    cout << " ";
    for (int i = 0; i < (dimensionX * dimensionY); i++) {
        cout << matrix[i] << " ";
        if (((i + 1) % dimensionX == 0) && (i != 0))
            cout << endl << " ";
    }
}

void visualiseMatrices(const int const* matrixInitial, const int const* matrixTransposed, size_t M, size_t N) {
    cout << endl; //<< " ";

    cout << "Initial matrix: " << endl;
    printMatrix(matrixInitial, M, N);
    cout << endl << endl;

    cout << "Result of transposing: " << endl;
    printMatrix(matrixTransposed, M, N);
    cout << endl << endl;
}

int* transposeMatrices_CPU(int* matrixInitial, size_t M, size_t N) {
    int* matrixTransposed = new int[M * N];

    for (int x = 0; x < M; ++x) {
        for (int y = 0; y < N; ++y) {
            matrixTransposed[y * M + x] = matrixInitial[x * N + y];
        }
    }
    return matrixTransposed;
}

void verify(int* matrixInitial, int* matrixTransposed, size_t M, size_t N) {
    auto startTime = chrono::high_resolution_clock::now();
    int* matrixTransposed_CPU = transposeMatrices_CPU(matrixInitial, M, N);
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime);

    cout << "CPU calculation time: " << duration.count() / 1000000.0 << " milliseconds." << endl;

    bool isEqual = true;
    for (int i = 0; i < (M * N); ++i) {
        if (matrixTransposed_CPU[i] != matrixTransposed[i]) {
            isEqual = false;
        }
    }

    free(matrixTransposed_CPU);

    if (isEqual) {
        cout << "Results are the same!" << endl;
    }
    else {
        cout << "Results are not the same!" << endl;
    }
}

void printInfoAboutDevice(cl_device_id device_id, size_t& maxWorkGroupSize) {
    cl_uint uintValue;
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(uintValue), &uintValue, NULL);
    cout << "Number of parallel compute units on the device: " << uintValue << endl;

    clGetDeviceInfo(device_id, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(uintValue), &uintValue, NULL);
    cout << "Max clock frequency of the device: " << uintValue << " MHz" << endl;

    clGetDeviceInfo(device_id, CL_DEVICE_ADDRESS_BITS, sizeof(uintValue), &uintValue, NULL);
    cout << "Device address space size: " << uintValue << endl;

    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(maxWorkGroupSize), &maxWorkGroupSize, NULL);
    cout << "Max number of work-items in a work-group that a device is capable of executing on a single compute unit: " << maxWorkGroupSize << endl;

    size_t dimensions[3];
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(dimensions), &dimensions, NULL);
    cout << "Max number the of work-items that can be specified in each dimension of the work-group: { " << dimensions[0] << ", " << dimensions[1] << ", " << dimensions[2] << " }" << endl;
}

void fillMatrix(int* matrix, const int size) {
    int max = 9;
    int min = 1;
    for (int i = 0; i < size; ++i) {
        matrix[i] = int(rand() % (max - min) + min);
    }
}

int main(int argc, char** argv) {

    // Reading and saving the arguments

    if (argc < 6) {
        cout << "You forgot some arguments! There should be 5: \n"
            << "2 (int) dimensions for the matrix (M, N), \n"
            << "1 (string) path to kernel file, \n"
            << "1 (bool) switch for verification, \n"
            << "1 (bool) switch for visualization. \n";
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    string pathToKernel = argv[3];
    bool verificationIsEnabled = atoi(argv[4]);
    bool visualisationIsEnabled = atoi(argv[5]);

    if (!M || !N) {
        cout << "Matrix should have non-zero dimensions! \n";
        return 1;
    }


    // Getting the platform, connecting to the GPU and printing some info about it

    cl_platform_id platform_id;
    clGetPlatformIDs(1, &platform_id, NULL);

    cl_int errorCode;

    cl_device_id device_id;
    errorCode = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

    size_t maxWorkGroupSize;

    printInfoAboutDevice(device_id, maxWorkGroupSize);


    // Loading kernel code, creating OpenCL context, creating & building the program

    ifstream sourceFile(pathToKernel);
    string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));

    char* kernelSource = new char[sourceCode.length() + 1];
    strcpy_s(kernelSource, sourceCode.size()+1, sourceCode.c_str());

    cl_context context = clCreateContext(0, 1, &device_id, NULL, NULL, &errorCode);

    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &errorCode);

    errorCode = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);


    // Creation of the initial and transposed matrices and memory buffers for them

    int* matrixInitial = (int*)malloc(sizeof(int) * (M * N));
    fillMatrix(matrixInitial, M * N);
    cl_mem bufferForInitial = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int) * (M * N), matrixInitial, &errorCode);

    int* matrixTransposed = (int*)malloc(sizeof(int) * (N * M));
    cl_mem bufferForTransposed = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * (N * M), NULL, &errorCode);


    // Creation of the kernel and setting of the kernel arguments
    cl_kernel kernel = clCreateKernel(program, KERNEL_NAME, &errorCode);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufferForInitial);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bufferForTransposed);
    clSetKernelArg(kernel, 2, sizeof(cl_int), (void*)&M);
    clSetKernelArg(kernel, 3, sizeof(cl_int), (void*)&N);


    // Setting local and global work-group sizes, creation of the command queue, queueing and execution of the kernel, showing results and timestamps

    size_t localWorkSize[2] = {
        sqrt(maxWorkGroupSize),
        sqrt(maxWorkGroupSize)
    };

    size_t globalWorkSize[2] = {
        ceil(double(M) / double(localWorkSize[0])) * localWorkSize[0],
        ceil(double(N) / double(localWorkSize[1])) * localWorkSize[1]
    };
    

    cl_queue_properties property[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, property, &errorCode);
    cl_event waitEvent;

    clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, &waitEvent);

    clWaitForEvents(1, &waitEvent);

    clFinish(command_queue);

    cl_ulong time_start;
    cl_ulong time_end;
    clGetEventProfilingInfo(waitEvent, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(waitEvent, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    unsigned long nanoSeconds = time_end - time_start;

    clEnqueueReadBuffer(command_queue, bufferForTransposed, CL_TRUE, 0, sizeof(int) * (N * M), matrixTransposed, 0, NULL, NULL);

    cout << "GPU calculation time: " << double(nanoSeconds) / 1000000.0 << " milliseconds." << endl;
    //cout << "start time: " << time_start << endl;
    //cout << "end time: " << time_end << endl;

    // Checking if the program is working incorrectly by comparing result to result of the usual CPU operation

    if (verificationIsEnabled)
        verify(matrixInitial, matrixTransposed, M, N);


    // Visualising matrices (WARNING! Can take awhile and look strange with big matrices)

    if (visualisationIsEnabled)
        visualiseMatrices(matrixInitial, matrixTransposed, M, N);


    // Cleaning memory

    free(matrixInitial);
    free(matrixTransposed);
    clReleaseMemObject(bufferForInitial);
    clReleaseMemObject(bufferForTransposed);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);

    return 0;
}