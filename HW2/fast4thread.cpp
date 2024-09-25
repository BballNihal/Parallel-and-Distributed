// Author : Nihal Abdul Muneer and Thomas Vu

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <immintrin.h>
#include <cmath>
using namespace std;

class prime_bits
{

private:
    uint64_t n;         // the number of bits
    uint64_t num_words; // the number of 64-bit words needed to store
    uint64_t *p;        // point to the dynamic memory for the bits

public:
    prime_bits(uint64_t n) : // round up because we need n+1 bits
                             n(n + 1), num_words(n / 64 + 1), p(new uint64_t[n / 64 + 1])
    {
    }
    ~prime_bits()
    {
        delete[] p;
    }

    // prevent you from accidentally copying bits. You really don't want to
    prime_bits(const prime_bits &orig) = delete;
    prime_bits &operator=(const prime_bits &orig) = delete;

    // let's store primes as 0 and not prime (composite) as 1 it's easier.
    // to set a bit we shift the mask to the right spot and OR it in
    void clear_prime(uint64_t i)
    {
        // this is storing only odd numbers in each mask!
        // note 1LL is crucial. If you write just 1 it's an int.
        // (1 << 40) would be 0   (1LL << 40) is 10000000000000000000000000....
        p[i / 128] |= (1LL << ((i % 128) >> 1));
    }
    bool is_prime(uint64_t i) const
    {
        return (p[i / 128] & (1LL << ((i % 128) >> 1))) == 0;
    }

    // use bitcounting to avoid counting each one separately
    uint64_t fast_eratosthenes()
    {
        uint64_t count = 1; // 2 is a special case
        uint64_t lim = sqrt(n);
        for (uint64_t i = 3; i <= lim; i += 2)
        {
            if (is_prime(i))
            {
                count++;
                for (uint64_t j = i * i; j <= n; j += 2 * i)
                {
                    clear_prime(j);
                }
            }
        }
        /*
            TODO: check if this this boundary condition is right.
            handle the few primes between sqrt(n) and the next 64-bit word boundary
        */
        uint64_t word_index = (lim + 127) / 128;
        for (uint64_t i = (lim + 1) | 1; i < word_index * 128; i++)
            clear_prime(i);

        lim = (lim + 127) / 128; // round up to next even word boundary

        for (uint64_t i = lim; i < num_words; i++)
            count += _mm_popcnt_u64(~p[i]); // count how many bits are set
        return count;
    }
};

int main()
{

    // initialize variables and array
    uint64_t n = 100;

    prime_bits primes(n);

    auto t0 = std::chrono::high_resolution_clock::now();
    uint64_t count = primes.fast_eratosthenes();
    auto t1 = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

    cout << "fast eratosthenes: " << count << " elapsed: " << elapsed.count() << "usec\n";

    // uint64_t count = 0,count1 = 0,count2 = 0,count3 = 0,count4 = 0;

    // thread t1(prime_bits::fast_eratosthenes, ref(primes), 2 , n/4, &count1);
    // thread t2(prime_bits::fast_eratosthenes, ref(primes), n/4+1, n/2, &count2);
    // thread t3(prime_bits::fast_eratosthenes, ref(primes), n/2+1, 3*n/4, &count3);
    // thread t4(prime_bits::fast_eratosthenes, ref(primes), 3*n/4+1, n, &count4);

    // //join threads
    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();

    // Print the primes
    // for (uint64_t i = 0; i <= n; i++) {
    //     if (primes[i]) {
    //         printf("%ld ", i);
    //     }
    // }

    // print count
    //  for (uint64_t i = 2; i <= n; i++) {
    //      if (primes[i]) {
    //          count++;
    //      }
    //  }
    //  count = count1 + count2 + count3 + count4;
    //  cout << "\n There are " << count << " primes\n";
}