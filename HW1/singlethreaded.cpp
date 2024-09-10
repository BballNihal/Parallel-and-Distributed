#include <stdio.h>
#include <stdint.h>
#include <stdbool.h> // C++ supports this header for bool type

void eratosthenes(bool primes[], uint64_t n) {
    uint64_t count = 0;

    // Initialize the primes array: mark all as prime initially
    for (uint64_t i = 0; i <= n; i++) {
        primes[i] = true;
    }

    // Mark 0 and 1 as non-prime (special cases)
    primes[0] = false;
    primes[1] = false;

    // Mark all even numbers (except 2) as non-prime
    for (uint64_t i = 4; i <= n; i += 2) {
        primes[i] = false;
    }

    // Sieve of Eratosthenes starting from 3 and skipping even numbers
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (primes[i]) {
            // Mark all odd multiples of i as non-prime
            for (uint64_t j = i * i; j <= n; j += 2 * i) {
                primes[j] = false;
            }
        }
    }

    // Count the number of primes
    for (uint64_t i = 2; i <= n; i++) {
        if (primes[i]) {
            count++;
        }
    }

    printf("Count of primes: %lu\n", count);
}

int main() {
    uint64_t n = 100000000; // 1 billion
    bool *primes = new bool[n + 1]; // Dynamically allocate the primes array

    if (primes == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Call the function to fill the primes array
    eratosthenes(primes, n);

    // Free the allocated memory
    delete[] primes;

    return 0;
}
