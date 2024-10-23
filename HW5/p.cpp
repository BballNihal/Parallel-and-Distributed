// Author: Nihal Abdul Muneer and Thomas Vu
// code benchmarks matrix multiplication using AVX intrinsics and OpenMP.
// 
// TEST RESULTS:
// Identity matrix with 1 threads took 1.13604 seconds.
// Identity matrix with 2 threads took 0.561119 seconds.
// Identity matrix with 4 threads took 0.446282 seconds.
// Identity matrix with 8 threads took 0.345184 seconds.
// Diagonal matrix with 1 threads took 1.07919 seconds.
// Diagonal matrix with 2 threads took 0.604673 seconds.
// Diagonal matrix with 4 threads took 0.377419 seconds.
// Diagonal matrix with 8 threads took 0.34895 seconds.

// Transposed Matrix Multiplication:
// Transposed identity matrix with 1 threads took 0.299618 seconds.
// Transposed identity matrix with 2 threads took 0.154547 seconds.
// Transposed identity matrix with 4 threads took 0.094149 seconds.
// Transposed identity matrix with 8 threads took 0.0739982 seconds.

// THIS IS DONE ON 4 CORE PROCESSOR WITH HYPERTHREADING (4 LOGICAL PROCESSORS)
//hyperthreading does result in a performance boost

#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <immintrin.h> // For AVX intrinsics
#include <cstdlib> // For rand()

using namespace std;

class Matrix {
public:
    vector<vector<double>> data;
    int size;

    Matrix() : size(0) {}

    Matrix(int s) : size(s) {
        data.resize(size, vector<double>(size, 0.0));
    }

    //create an identity matrix
    void identity() {
        for (int i = 0; i < size; ++i) {
            data[i][i] = 1.0;
        }
    }

    //create a matrix with diagonal=3, others=2
    void diagonalMatrix() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                data[i][j] = (i == j) ? 3.0 : 2.0;
            }
        }
    }

    // Multithreaded matrix multiplication using AVX intrinsics
    Matrix multithreadedMultiplyAVX(const Matrix& other, int num_threads) const {
        Matrix result(size);
        #pragma omp parallel for num_threads(num_threads) collapse(2)
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; j += 4) { //Process 4 elements at a time using AVX
                __m256d c = _mm256_setzero_pd(); 
                for (int k = 0; k < size; ++k) {
                    // Load 4 elements of the B matrix and multiply wuth correspondiong A Vvector
                    __m256d b = _mm256_loadu_pd(&other.data[k][j]);
                    __m256d a = _mm256_set1_pd(data[i][k]); //Broadcast A[i][k]
                    c = _mm256_fmadd_pd(a, b, c); // Multiply and add 
                }
                _mm256_storeu_pd(&result.data[i][j], c); //result vector
            }
        }
        return result;
    }

    // Matrix multiplication using transposed B
    Matrix multiplyWithTransposedAVX(const Matrix& B, int num_threads) const {
        Matrix result(size);
        Matrix B_transposed = B.transpose();
        #pragma omp parallel for num_threads(num_threads) collapse(2)
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; j += 4) { 
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

//benchmarking
void benchmark(const Matrix& A, const Matrix& B, int num_threads, const string& desc, bool use_transpose = false) {
    auto start = chrono::high_resolution_clock::now();
    Matrix C;

    //check for transpose
    if (use_transpose) {
        C = A.multiplyWithTransposedAVX(B, num_threads);
    } else {
        C = A.multithreadedMultiplyAVX(B, num_threads);
    }

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

    //transposed 
    cout << "\nTransposed Matrix Multiplication:\n";
    benchmark(A, B, 1, "Transposed identity matrix", true);
    benchmark(A, B, 2, "Transposed identity matrix", true);
    benchmark(A, B, 4, "Transposed identity matrix", true);
    benchmark(A, B, 8, "Transposed identity matrix", true);

    return 0;
}
