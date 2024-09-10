#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm> // for std::max

using namespace std;

void eratosthenes(vector<bool>& primes, uint64_t start, uint64_t end) {
    if (start < 2) start = 2; // Ensure start is at least 2

    // Sieve of Eratosthenes
    for (uint64_t i = 2; i <= sqrt(end); i++) {
        if (primes[i]) {
            // Start from the maximum of i*i or the first multiple in the range
            for (uint64_t j = max(i * i, (start + i - 1) / i * i); j <= end; j += i) {
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
    vector<bool> primes(n + 1, true); // Initializes all elements to true
    primes[0] = primes[1] = false; // 0 and 1 are not primes

    uint64_t mid = n / 2;

    thread t1(thread_func, ref(primes), 2, mid); //start from 2 since 0,1 not prime
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
