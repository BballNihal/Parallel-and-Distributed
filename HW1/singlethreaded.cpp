#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25
// 0  0  1  0  1  0  1  1  1   0   1   0   1   1   1   0                               1
// i=97
//O(log log n)

void eratosthenes(bool primes[], uint64_t n) {
    uint64_t count = 0;

    // Initialize the primes array
    for (uint64_t i = 2; i <= n; i++) {
        primes[i] = true;
    }

    // Sieve of Eratosthenes
    for (uint64_t i = 2; i <= n; i++) {
        if (primes[i]) {
            count++;
            for (uint64_t j = i * i; j <= n; j += 2*i) {
                primes[j] = false;
            }
        }
    }
    // Optional: Print the count of primes
    // printf("Count of primes: %lu\n", count);
}

int main() {
    uint64_t n = 1000000000;
    bool primes[n+1];

    // Call the function to fill the primes array
    eratosthenes(primes, n);

    // Print the primes
    for (uint64_t i = 2; i <= n; i++) {
        if (primes[i]) {
            printf("%lu ", i);
        }
    }
    printf("\n");

    return 0;
}
