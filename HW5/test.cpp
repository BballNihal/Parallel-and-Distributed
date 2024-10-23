// Author: Nihal Abdul Muneer and Thomas Vu

#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <immintrin.h> 
#include <cstdlib> 

using namespace std;

class Matrix {
public:
    vector<vector<double>> data;
    int size;

    Matrix(int s) : size(s) {
        data.resize(size, vector<double>(size, 0.0));
    }

    void identity() {
        for (int i = 0; i < size; ++i) {
            data[i][i] = 1.0;
        }
    }

    void diagonalMatrix() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                data[i][j] = (i == j) ? 3.0 : 2.0;
            }
        }
    }

    Matrix multithreadedMultiplyAVX(const Matrix& other, int num_threads) const {
        Matrix result(size);
        #pragma omp parallel for num_threads(num_threads) collapse(2)
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; j += 4) { 
                __m256d c = _mm256_setzero_pd(); 
                for (int k = 0; k < size; ++k) {
                    // Load 4 elements of the B matrix (other) row-wise and multiply them with the corresponding A matrix element
                    __m256d b = _mm256_loadu_pd(&other.data[k][j]);
                    __m256d a = _mm256_set1_pd(data[i][k]); // Broadcast A[i][k] to all elements of the vector
                    c = _mm256_fmadd_pd(a, b, c); // Multiply and add to the result vector
                }
                _mm256_storeu_pd(&result.data[i][j], c); // Store the result vector
            }
        }
        return result;
    }

    // Matrix multiplication using transposed B for better cache locality
    Matrix multiplyWithTransposedAVX(const Matrix& B, int num_threads) const {
        Matrix result(size);
        Matrix B_transposed = B.transpose();
        #pragma omp parallel for num_threads(num_threads) collapse(2)
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; j += 4) { //4 elements at a time using AVX
                __m256d c = _mm256_setzero_pd();
                for (int k = 0; k < size; ++k) {
                    __m256d b = _mm256_loadu_pd(&B_transposed.data[j][k]); 
                    __m256d a = _mm256_set1_pd(data[i][k]);
                    c = _mm256_fmadd_pd(a, b, c);
                }
                _mm256_storeu_pd(&result.data[i][j], c);
            }
        }
        return result;
    }

    Matrix transpose() const {
        Matrix transposed(size);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                transposed.data[j][i] = data[i][j];
            }
        }
        return transposed;
    }
};

//henchmarking
void benchmark(Matrix& A, Matrix& B, int num_threads, const string& desc) {
    auto start = chrono::high_resolution_clock::now();
    Matrix C = A.multithreadedMultiplyAVX(B, num_threads);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << desc << " with " << num_threads << " threads took " << duration.count() << " seconds." << endl;
}

int main() {
    int size = 1024;
    Matrix A(size), B(size);

    A.identity();
    B.identity();
    
    Matrix D(size);
    D.diagonalMatrix();
    
    //identity
    benchmark(A, B, 1, "Identity matrix");
    benchmark(A, B, 2, "Identity matrix");
    benchmark(A, B, 4, "Identity matrix");
    benchmark(A, B, 8, "Identity matrix");
    
    //diagonal
    benchmark(D, D, 1, "Diagonal matrix");
    benchmark(D, D, 2, "Diagonal matrix");
    benchmark(D, D, 4, "Diagonal matrix");
    benchmark(D, D, 8, "Diagonal matrix");

    //transporse
    cout << "\nTransposed Matrix Multiplication:\n";
    benchmark(A, B, 1, "Transpose identity matrix");
    benchmark(A, B, 2, "Transpose identity matrix");
    benchmark(A, B, 4, "Transpose identity matrix");
    benchmark(A, B, 8, "Transpose identity matrix");

    return 0;
}
