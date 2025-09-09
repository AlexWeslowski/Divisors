// Divisors.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cstdint>
#include <bit>
#include <cmath>
#include <algorithm>
#include <numeric> 
#include <bitset>
#include <utility>
#include <limits>
#include <random>
#include <source_location>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <../include/ValidInteger.h>
#include <../include/Builtins.h>
#include <../include/Stringify.h>
#include <../include/Globals.h>
#include <../include/primesieve.hpp>

#ifndef DIVISORS_H
#define DIVISORS_H

static bool bln_init = false;
// 2^24 = 16777216
// 2^25 = 33554432
// 2^26 = 67108864
const int LEN_PRIMES = 67108864;
const int MAX_RECURSION = 10;

static std::vector<std::map<int64_t, int>> small_factor_cache(100);
static std::map<int64_t, int> factor_cache;
static std::bitset<67108864 + 1> setprimes;
static std::vector<int64_t> aryprimes;
static std::map<int64_t, size_t> idxprimes;


template<ValidIntegerType T>
class Divisors {
private:
    /*
    static std::vector<std::map<T, int>> small_factor_cache;
    static std::map<T, int> factor_cache;
    static std::bitset<67108864 + 1> setprimes;
    static std::vector<T> aryprimes;
    static std::map<T, size_t> idxprimes;
    */

    /*
    Divisors() {
        if (bln_init) {
            return;
        }
        import sympy
        import math
        for n in range(0, 100):
            m = n
            for p in sympy.sieve.primerange(n + 1):
                i = 0
                while m % p == 0:
                    m //= p
                    i += 1
                if i > 0:
                    print(f"small_factor_cache[{n}][{p}] = {i};")
        small_factor_cache[0][1] = 1;
        small_factor_cache[2][2] = 1;
        small_factor_cache[3][3] = 1;
        small_factor_cache[4][2] = 2;
        small_factor_cache[5][5] = 1;
        small_factor_cache[6][2] = 1;
        small_factor_cache[6][3] = 1;
        small_factor_cache[7][7] = 1;
        small_factor_cache[8][2] = 3;
        small_factor_cache[9][3] = 2;
        small_factor_cache[10][2] = 1;
        small_factor_cache[10][5] = 1;
        small_factor_cache[11][11] = 1;
        small_factor_cache[12][2] = 2;
        small_factor_cache[12][3] = 1;
        small_factor_cache[13][13] = 1;
        small_factor_cache[14][2] = 1;
        small_factor_cache[14][7] = 1;
        small_factor_cache[15][3] = 1;
        small_factor_cache[15][5] = 1;
        small_factor_cache[16][2] = 4;
        small_factor_cache[17][17] = 1;
        small_factor_cache[18][2] = 1;
        small_factor_cache[18][3] = 2;
        small_factor_cache[19][19] = 1;
        small_factor_cache[20][2] = 2;
        small_factor_cache[20][5] = 1;
        small_factor_cache[21][3] = 1;
        small_factor_cache[21][7] = 1;
        small_factor_cache[22][2] = 1;
        small_factor_cache[22][11] = 1;
        small_factor_cache[23][23] = 1;
        small_factor_cache[24][2] = 3;
        small_factor_cache[24][3] = 1;
        small_factor_cache[25][5] = 2;
        small_factor_cache[26][2] = 1;
        small_factor_cache[26][13] = 1;
        small_factor_cache[27][3] = 3;
        small_factor_cache[28][2] = 2;
        small_factor_cache[28][7] = 1;
        small_factor_cache[29][29] = 1;
        small_factor_cache[30][2] = 1;
        small_factor_cache[30][3] = 1;
        small_factor_cache[30][5] = 1;
        small_factor_cache[31][31] = 1;
        small_factor_cache[32][2] = 5;
        small_factor_cache[33][3] = 1;
        small_factor_cache[33][11] = 1;
        small_factor_cache[34][2] = 1;
        small_factor_cache[34][17] = 1;
        small_factor_cache[35][5] = 1;
        small_factor_cache[35][7] = 1;
        small_factor_cache[36][2] = 2;
        small_factor_cache[36][3] = 2;
        small_factor_cache[37][37] = 1;
        small_factor_cache[38][2] = 1;
        small_factor_cache[38][19] = 1;
        small_factor_cache[39][3] = 1;
        small_factor_cache[39][13] = 1;
        small_factor_cache[40][2] = 3;
        small_factor_cache[40][5] = 1;
        small_factor_cache[41][41] = 1;
        small_factor_cache[42][2] = 1;
        small_factor_cache[42][3] = 1;
        small_factor_cache[42][7] = 1;
        small_factor_cache[43][43] = 1;
        small_factor_cache[44][2] = 2;
        small_factor_cache[44][11] = 1;
        small_factor_cache[45][3] = 2;
        small_factor_cache[45][5] = 1;
        small_factor_cache[46][2] = 1;
        small_factor_cache[46][23] = 1;
        small_factor_cache[47][47] = 1;
        small_factor_cache[48][2] = 4;
        small_factor_cache[48][3] = 1;
        small_factor_cache[49][7] = 2;
        small_factor_cache[50][2] = 1;
        small_factor_cache[50][5] = 2;
        small_factor_cache[51][3] = 1;
        small_factor_cache[51][17] = 1;
        small_factor_cache[52][2] = 2;
        small_factor_cache[52][13] = 1;
        small_factor_cache[53][53] = 1;
        small_factor_cache[54][2] = 1;
        small_factor_cache[54][3] = 3;
        small_factor_cache[55][5] = 1;
        small_factor_cache[55][11] = 1;
        small_factor_cache[56][2] = 3;
        small_factor_cache[56][7] = 1;
        small_factor_cache[57][3] = 1;
        small_factor_cache[57][19] = 1;
        small_factor_cache[58][2] = 1;
        small_factor_cache[58][29] = 1;
        small_factor_cache[59][59] = 1;
        small_factor_cache[60][2] = 2;
        small_factor_cache[60][3] = 1;
        small_factor_cache[60][5] = 1;
        small_factor_cache[61][61] = 1;
        small_factor_cache[62][2] = 1;
        small_factor_cache[62][31] = 1;
        small_factor_cache[63][3] = 2;
        small_factor_cache[63][7] = 1;
        small_factor_cache[64][2] = 6;
        small_factor_cache[65][5] = 1;
        small_factor_cache[65][13] = 1;
        small_factor_cache[66][2] = 1;
        small_factor_cache[66][3] = 1;
        small_factor_cache[66][11] = 1;
        small_factor_cache[67][67] = 1;
        small_factor_cache[68][2] = 2;
        small_factor_cache[68][17] = 1;
        small_factor_cache[69][3] = 1;
        small_factor_cache[69][23] = 1;
        small_factor_cache[70][2] = 1;
        small_factor_cache[70][5] = 1;
        small_factor_cache[70][7] = 1;
        small_factor_cache[71][71] = 1;
        small_factor_cache[72][2] = 3;
        small_factor_cache[72][3] = 2;
        small_factor_cache[73][73] = 1;
        small_factor_cache[74][2] = 1;
        small_factor_cache[74][37] = 1;
        small_factor_cache[75][3] = 1;
        small_factor_cache[75][5] = 2;
        small_factor_cache[76][2] = 2;
        small_factor_cache[76][19] = 1;
        small_factor_cache[77][7] = 1;
        small_factor_cache[77][11] = 1;
        small_factor_cache[78][2] = 1;
        small_factor_cache[78][3] = 1;
        small_factor_cache[78][13] = 1;
        small_factor_cache[79][79] = 1;
        small_factor_cache[80][2] = 4;
        small_factor_cache[80][5] = 1;
        small_factor_cache[81][3] = 4;
        small_factor_cache[82][2] = 1;
        small_factor_cache[82][41] = 1;
        small_factor_cache[83][83] = 1;
        small_factor_cache[84][2] = 2;
        small_factor_cache[84][3] = 1;
        small_factor_cache[84][7] = 1;
        small_factor_cache[85][5] = 1;
        small_factor_cache[85][17] = 1;
        small_factor_cache[86][2] = 1;
        small_factor_cache[86][43] = 1;
        small_factor_cache[87][3] = 1;
        small_factor_cache[87][29] = 1;
        small_factor_cache[88][2] = 3;
        small_factor_cache[88][11] = 1;
        small_factor_cache[89][89] = 1;
        small_factor_cache[90][2] = 1;
        small_factor_cache[90][3] = 2;
        small_factor_cache[90][5] = 1;
        small_factor_cache[91][7] = 1;
        small_factor_cache[91][13] = 1;
        small_factor_cache[92][2] = 2;
        small_factor_cache[92][23] = 1;
        small_factor_cache[93][3] = 1;
        small_factor_cache[93][31] = 1;
        small_factor_cache[94][2] = 1;
        small_factor_cache[94][47] = 1;
        small_factor_cache[95][5] = 1;
        small_factor_cache[95][19] = 1;
        small_factor_cache[96][2] = 5;
        small_factor_cache[96][3] = 1;
        small_factor_cache[97][97] = 1;
        small_factor_cache[98][2] = 1;
        small_factor_cache[98][7] = 2;
        small_factor_cache[99][3] = 2;
        small_factor_cache[99][11] = 1;


        setprimes[0] = false;
        setprimes[1] = false;
        setprimes[2] = true;
        setprimes[3] = true;
        setprimes[4] = false;

        primesieve::iterator it;
        uint64_t p = it.next_prime();
        for (; p < LEN_PRIMES; p = it.next_prime()) {
            aryprimes.push_back(p);
            setprimes[p] = true;
        }

        //std::vector<int> aryprimes;
        //primesieve::generate_primes(LEN_PRIMES, &aryprimes);

        std::bitset<LEN_PRIMES + 1> isprime;
        isprime.set();

        isprime[0] = false;
        isprime[1] = false;
        isprime[2] = true;
        isprime[3] = true;
        isprime[4] = false;
        if (false && verbose) std::cout << "Line " << std::source_location::current().line() << std::endl;
        for (int ip = 2 * 2; ip < LEN_PRIMES; ip += 2) {
            isprime[ip] = false;
        }
        if (false && verbose) std::cout << "Line " << std::source_location::current().line() << std::endl;
        for (int ip = 3 * 3; ip < LEN_PRIMES; ip += 3) {
            isprime[ip] = false;
        }

        if (false && verbose) std::cout << "Line " << std::source_location::current().line() << std::endl;
        if (Globals::verbose) std::cout << "LEN_PRIMES = " << LEN_PRIMES << std::endl;
        if (Globals::verbose) std::cout << "isprime[46351] = " << isprime[46351] << std::endl;
        int SQRT_LEN_PRIMES = isqrt3a(LEN_PRIMES);
        for (int i = 5; i < LEN_PRIMES; i += 2) {
            if (false && verbose) std::cout << "isprime[" << i << "] = " << isprime[i] << std::endl;
            if (isprime[i]) {
                for (int p = 3; p <= isqrt3a(i); p += 2) {
                    if (setprimes[p] && i % p == 0) {
                        isprime[i] = false;
                        break;
                    }
                }
            }
            if (false && verbose) std::cout << "isprime[" << i << "] = " << isprime[i] << std::endl;
            if (isprime[i]) {
                setprimes[i] = true;
                if (i <= SQRT_LEN_PRIMES) {
                    for (int i2 = i * i; i2 < LEN_PRIMES; i2 += i) {
                        isprime[i2] = false;
                    }
                }
            }
        }

        std::vector<int> primes;
        for (int p = 2; p <= limit; ++p) {
            if (is_prime[p]) {
                primes.push_back(p);
            }
        }

        bln_init = true;
    }
    */

public:
    Divisors();
    static Divisors<T>& get_instance();
    void set_verbose(bool bln);
    constexpr int __countr_zero(T __x) noexcept;
    int bit_scan1(T n);
    int bit_length(T n);
    T integer_pow(T base, int exp);
    std::pair<T, bool> iroot(T n, int p);
    std::pair<T, T> sqrtrem(T N);
    static T isqrt3a(T N);
    static std::pair<T, T> isqrt3b(T N, T Low, T High);
    static T isqrt5(T n);
    int num_digits(T n);
    bool is_prime(T n);
    std::pair<T, int> remove(T n, T p);
    std::vector<T> divisors(T n);
    std::vector<T> _rec_gen(T n, const std::map<T, int>& factors, const std::vector<T>& keys);
    std::vector<T> _divisors(T n);
    std::pair<T, T> _factorint_small(std::map<T, int>& factors, T n, T limit, int fail_max, T next_p);
    bool _check_termination(std::map<T, int>& factors, T n, T next_p, int call_depth);
    std::pair<T, bool> _trial(std::map<T, int>& factors, T n, const std::vector<T>& candidates);
    std::pair<T, int> _perfect_power_done(T current_n, std::map<T, int>& current_factors, int current_g, int current_multi);
    std::pair<T, int> _perfect_power(T n, T next_p, int call_depth);
    size_t find(T p);
    std::map<T, int> factorint(T n, int call_depth);
    T pollard_pm1(T n, int B = 10, T a = 2, int retries = 0, unsigned int seed = 1234);
    T power_mod(T base, T exp, T mod);
    T pollard_rho_f(T x, T n2, T a2);
    T pollard_rho(T n1, T s = 2, T a1 = 1, int retries = 5, unsigned int seed = 1234, int64_t max_steps = 0);
};


//template<ValidIntegerType T>
//std::vector<std::map<int64_t, int>> Divisors<T>::small_factor_cache(100);


#endif