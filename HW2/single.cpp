#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

void simpleSieve(uint64_t limit, vector<uint8_t>& primes) {
    for (uint64_t i = 0; i <= limit / 2; i++) {
        primes[i / 8] |= (1 << (i % 8));
    }

    for (uint64_t p = 3; p * p <= limit; p += 2) {
        if (primes[p / 16] & (1 << ((p / 2) % 8))) {
            for (uint64_t i = p * p; i <= limit; i += 2 * p) {
                primes[i / 16] &= ~(1 << ((i / 2) % 8));
            }
        }
    }
}

uint64_t segmentedSieve(uint64_t n) {
    uint64_t limit = floor(sqrt(n)) + 1;
    vector<uint8_t> primes((limit / 16) + 1, 0);
    simpleSieve(limit, primes);

    uint64_t count = 1; // Counting the prime number 2
    for (uint64_t p = 3; p <= limit; p += 2) {
        if (primes[p / 16] & (1 << ((p / 2) % 8))) {
            count++;
        }
    }

    uint64_t low = limit;
    uint64_t high = 2 * limit;

    while (low < n) {
        if (high >= n) high = n;

        vector<uint8_t> mark((limit / 16) + 1, 0);
        for (uint64_t i = 0; i <= (high - low) / 2; i++) {
            mark[i / 8] |= (1 << (i % 8));
        }

        for (uint64_t i = 3; i <= limit; i += 2) {
            if (primes[i / 16] & (1 << ((i / 2) % 8))) {
                uint64_t loLim = max(i * i, (low + i - 1) / i * i);
                if (loLim % 2 == 0) loLim += i;

                for (uint64_t j = loLim; j < high; j += 2 * i) {
                    mark[(j - low) / 16] &= ~(1 << (((j - low) / 2) % 8));
                }
            }
        }

        for (uint64_t i = low; i < high; i += 2) {
            if (mark[(i - low) / 16] & (1 << (((i - low) / 2) % 8))) {
                count++;
            }
        }

        low += limit;
        high += limit;
    }

    return count;
}

int main() {
    uint64_t n = 1000000000;
    uint64_t primeCount = segmentedSieve(n);
    cout << "Count of primes: " << primeCount << endl;
    return 0;
}
