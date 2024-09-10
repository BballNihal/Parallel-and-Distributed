#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm> // for std::max

using namespace std;

void eratosthenes(vector<bool>& primes, uint64_t start, uint64_t end) {
    if (start < 2) start = 2; // Ensure we start from at least 2

    // Mark multiples of each prime
    for (uint64_t i = 2; i <= end; i++) {
        if (primes[i]) {
            // Start marking from i*i
            for (uint64_t j = i * i; j <= end; j += 2 * i) {
                primes[j] = false;
            }
        }
    }
}

void thread_func(vector<bool>& primes, uint64_t start, uint64_t end) {
    eratosthenes(primes, start, end);
}

int main() {
    uint64_t n = 100;
    vector<bool> primes(n + 1, true); // Initialize all to true
    primes[0] = primes[1] = false; // 0 and 1 are not primes

    // Special case for the number 2, which is prime
    if (n >= 2) primes[2] = true;

    uint64_t mid = n / 2;

    thread t1(thread_func, ref(primes), 2, mid); // Start from 2
    thread t2(thread_func, ref(primes), mid + 1, n);

    t1.join();
    t2.join();

    uint64_t count = 0;
    for (uint64_t i = 2; i <= n; i++) {
        if (primes[i]) {
            count++;
        }
    }

    // Print the primes
    for (uint64_t i = 2; i <= n; i++) {
        if (primes[i]) {
            cout << i << " ";
        }
    }
    cout << "\n";

    cout << "# of primes up to " << n << ": " << count << '\n';

    return 0;
}
