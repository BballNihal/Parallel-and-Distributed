#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm> // for std::max

using namespace std;

uint64_t eratosthenes(vector<bool>& primes, uint64_t start, uint64_t end) {
    if (start < 2) start = 2; // Ensure we start from at least 2

    uint64_t count = 0;

    for (uint64_t i = 2; i <= end; i++) {
        if (primes[i]) {
            if (i >= start) count++; // Count prime numbers in range

            // Mark multiples of i as false
            for (uint64_t j = max(i * i, (start + i - 1) / i * i); j <= end; j += i) {
                primes[j] = false;
            }
        }
    }

    return count;
}

void thread_func(vector<bool>& primes, uint64_t start, uint64_t end, uint64_t* pcount) {
    *pcount = eratosthenes(primes, start, end);
}

int main() {
    uint64_t n = 1000000000;
    vector<bool> primes(n + 1, true); 
    primes[0] = primes[1] = false; 

    if (n >= 2) primes[2] = true;

    uint64_t mid = n / 2;
    uint64_t count1 = 0, count2 = 0;

    thread t1(thread_func, ref(primes), 2, mid, &count1);
    thread t2(thread_func, ref(primes), mid + 1, n, &count2);

    t1.join();
    t2.join();

    uint64_t count = count1 + count2;

    // Print the primes
    // for (uint64_t i = 2; i <= n; i++) {
    //     if (primes[i]) {
    //         cout << i << " ";
    //     }
    // }

    // Print the number of primes
    cout << "\n# of primes up to " << n << ": " << count << '\n';

    return 0;
}
