#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

void fillRandom(int **matrix, int size) {
    srand(time(0));
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            matrix[i][j] = rand() % 10; // Generate random numbers between 0 and 9
        }
    }
}

void multiplyMatrices(int **matrix1, int **matrix2, int **result, int size) {
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            result[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2)
    {
        std::cout << "usage: " << "./matrixMultSingleIneff <size of matrix>" << std::endl;
        return 1;
    }
    int n = atoi(argv[1]);

    // Allocate memory for matrices
    int **matrix1 = new int*[n];
    int **matrix2 = new int*[n];
    int **result = new int*[n];
    for (int i = 0; i < n; ++i) {
        matrix1[i] = new int[n];
        matrix2[i] = new int[n];
        result[i] = new int[n];
    }

    // Fill matrices with random numbers
    fillRandom(matrix1, n);
    fillRandom(matrix2, n);

    // Multiply matrices
    multiplyMatrices(matrix1, matrix2, result, n);

    std::ofstream outFile("outputMatrix.bin");
    for (int i = 0; i < n; ++i) {
        outFile.write((char*)result[i], n*sizeof(int));
    }
    outFile.close();

    // Deallocate memory
    for (int i = 0; i < n; ++i) {
        delete[] matrix1[i];
        delete[] matrix2[i];
        delete[] result[i];
    }
    delete[] matrix1;
    delete[] matrix2;
    delete[] result;

    return 0;
}