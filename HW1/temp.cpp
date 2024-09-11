#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <thread> 
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

    //0 1 not prime
    primes[0] = false;
    primes[1] = false;

    //all evens set prime
    for (uint64_t i = 4; i <= n; i += 2) {
        primes[i] = false;
    }

    uint64_t sqrt_n = static_cast<uint64_t>(sqrt(n));

    // Use two threads to mark non-prime numbers
    std::thread t1(mark_non_primes, primes, 3, n / 2, n);
    std::thread t2(mark_non_primes, primes, n / 2 + 1, n, n);

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
    uint64_t n = 1000000000; 
    bool *primes = new bool[n + 1]; 

    if (primes == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }


    eratosthenes(primes, n);

    delete[] primes;

    return 0;
}
