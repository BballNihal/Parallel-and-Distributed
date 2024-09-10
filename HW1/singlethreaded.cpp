#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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
            for (uint64_t j = i * i; j <= n; j += i) {
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
