//Author : Nihal Abdul Muneer and Thomas Vu

#include <iostream>
#include <cmath>
using namespace std;

void eratosthenes(bool primes[], uint64_t n) {
    uint64_t count = (n / 2) + 1; //odd numbers only. account for 2 here
    cout << count << endl;

    // set all prime
    for (uint64_t i = 0; i <= n / 2; i++) {
        primes[i] = true;
    }

    //set 0 prime
    primes[0] = false;
    count--;
    cout << count << endl;

    //start iterating from value 3
    for (uint64_t i = 1; 2 * i + 1 <= n; i++) {
        if (primes[i]) {
            uint64_t prime = 2 * i + 1;
            //all odd mults of prime are non prime
            for (uint64_t j = prime * prime; j <= n; j += 2 * prime) {
                    if (primes[j/2] == true){
                        primes[j / 2] = false;
                        count--;
                    }
            }
        }
    }

    //print all primes
    // cout << "2" << " ";
    // for (uint64_t i = 0; i <= n/2 -1; i++) {
    //     if (primes[i]) {
    //         cout << 2*i+1 << " ";
    //     }
    // }

    //print count
    printf("Count of primes: %lu\n", count);
}

int main() {
    uint64_t n = 1000000000;
    bool *primes = new bool[(n / 2) + 1]; 
    eratosthenes(primes, n);
    return 0;

    delete primes;
}
