#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <thread> // Include the thread library
#include <math.h>

void mark_non_primes(bool primes[], uint64_t start, uint64_t end, uint64_t n) {
    for (uint64_t i = start; i <= end; i += 2) {
        if (i * i <= n && primes[i]) {
            for (uint64_t j = i * i; j <= n; j += 2 * i) {
                primes[j] = false;
            }
        }
    }
}

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

    uint64_t sqrt_n = static_cast<uint64_t>(sqrt(n));

    // Use two threads to mark non-prime numbers
    std::thread t1(mark_non_primes, primes, 3, sqrt_n / 2, n);
    std::thread t2(mark_non_primes, primes, sqrt_n / 2 + 1, sqrt_n, n);

    t1.join();
    t2.join();

    // Count the number of primes
    for (uint64_t i = 2; i <= n; i++) {
        if (primes[i]) {
            count++;
        }
    }

    printf("Count of primes: %lu\n", count);
}

int main() {
    uint64_t n = 1000000000; // 1 billion
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
