#include <cstdlib>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <iostream>

using namespace std;

void fillRandom(int **matrix, int size) {
    srand(time(0));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 10; // Generate random numbers between 0 and 9
        }
    }
}

void multiplyMatrices(int **matrix1, int **matrix2, int **result, int size, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < size; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

int main(int argc, char** argv) {

    if (argc != 3)
    {
        std::cout << "usage: " << "./matrixMultFork <size of matrix> <num of processes>" << std::endl;
        return 1;
    }
    int n = atoi(argv[1]);
    int num_processes = atoi(argv[2]);

    // Allocate memory for matrices in shared memory
    int **matrix1 = reinterpret_cast<int**>(mmap(NULL, n * sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
    int **matrix2 = reinterpret_cast<int**>(mmap(NULL, n * sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
    int **result = reinterpret_cast<int**>(mmap(NULL, n * sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));

    for (int i = 0; i < n; ++i) {
        matrix1[i] = reinterpret_cast<int*>(mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
        matrix2[i] = reinterpret_cast<int*>(mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
        result[i] = reinterpret_cast<int*>(mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
    }

    // Fill matrices with random numbers
    fillRandom(matrix1, n);
    fillRandom(matrix2, n);

    // Fork processes for matrix multiplication
    int rows_per_process = n / num_processes;
    for (int i = 0; i < num_processes; ++i) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            int startRow = i * rows_per_process;
            int endRow = (i == num_processes - 1) ? n : (i + 1) * rows_per_process;
            multiplyMatrices(matrix1, matrix2, result, n, startRow, endRow);
            exit(0); // Exit child process after computation
        } else if (pid < 0) { // Error
            cerr << "Fork failed!" << endl;
            exit(1);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_processes; ++i) {
        wait(NULL);
    }

    std::ofstream outFile("outputMatrix.bin");
    for (int i = 0; i < n; ++i) {
        outFile.write((char*)result[i], n*sizeof(int));
    }
    outFile.close();

    // Deallocate memory
    for (int i = 0; i < n; ++i) {
        munmap(matrix1[i], n * sizeof(int));
        munmap(matrix2[i], n * sizeof(int));
        munmap(result[i], n * sizeof(int));
    }

    munmap(matrix1, n * sizeof(int*));
    munmap(matrix2, n * sizeof(int*));
    munmap(result, n * sizeof(int*));

    return 0;
}