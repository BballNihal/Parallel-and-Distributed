#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
using namespace std;

void eratosthenesMultithread(vector<bool>& primes, uint64_t a ,uint64_t b, uint64_t* pcount) {
    uint64_t count = 0;
    for (uint64_t i = 2; i <= b; i++) {
        if (primes[i]) {
            if(i >= a){
                count++;
            }
            for (uint64_t j = max(i * i, (a + i - 1) / i * i); j <= b; j += i) {
                primes[j] = false;
            }
        }
    }

    *pcount = count;
}

int main() {

    //initialize variables and array
    uint64_t n = 1000000000;
    vector<bool> primes(n+1, false);
    for (uint64_t i = 2; i <= n; i++) {
        primes[i] = true;
    }
    uint64_t count = 0,count1 = 0,count2 = 0,count3 = 0,count4 = 0;

    thread t1(eratosthenesMultithread, ref(primes), 2 , n/4, &count1);
    thread t2(eratosthenesMultithread, ref(primes), n/4+1, n/2, &count2);
    thread t3(eratosthenesMultithread, ref(primes), n/2+1, 3*n/4, &count3);
    thread t4(eratosthenesMultithread, ref(primes), 3*n/4+1, n, &count4);

    //join threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // Print the primes
    // for (uint64_t i = 0; i <= n; i++) {
    //     if (primes[i]) {
    //         printf("%ld ", i);
    //     }
    // }

    //print count
    // for (uint64_t i = 2; i <= n; i++) {
    //     if (primes[i]) {
    //         count++;
    //     }
    // }
    count = count1 + count2 + count3 + count4;
    cout << "\n There are " << count << " primes\n";
}