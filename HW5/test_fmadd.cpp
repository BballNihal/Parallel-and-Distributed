#include <immintrin.h>
#include <iostream>

int main() {
    __m256d a = _mm256_set1_pd(1.0);
    __m256d b = _mm256_set1_pd(2.0);
    __m256d c = _mm256_set1_pd(3.0);
    
    c = _mm256_fmadd_pd(a, b, c); 

    double result[4];
    _mm256_storeu_pd(result, c);
    
    std::cout << "Result: ";
    for (int i = 0; i < 4; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
