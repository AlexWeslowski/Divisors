#include "../include/ArrayArray.h"
#include "../include/Divisors.h"
#include <../include/Combinations.h>
using namespace std;

/*
class Foo {
    public:
        void bar() {
            std::cout << "Hello" << std::endl;
        }
};
*/


namespace py = pybind11;


namespace __gnu_cxx
{
    template<typename T>
    struct __int_traits
    {
        static constexpr int __digits = std::numeric_limits<T>::digits;
        static constexpr T __max = std::numeric_limits<T>::max();
    };
}


template<typename T>
std::string to_string(const std::vector<T>& vec) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1)
            oss << ", ";
    }
    oss << "]";
    return oss.str();
}

template<typename T1, typename T2>
std::string to_string(const std::map<T1, T2>& map) {
    std::ostringstream oss;
    oss << "{";
    auto it = map.begin();
    while (it != map.end()) {
        oss << it->first << ": " << it->second;
        ++it;
        if (it != map.end()) {
            oss << ", ";
        }
    }
    oss << "}";
    return oss.str();
}

template<ValidIntegerType T>
Divisors<T>::Divisors() {
    if (bln_init) {
        return;
    }
    /*
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
    */
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

    /*
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
    */

    /*
    std::vector<int> primes;
    for (int p = 2; p <= limit; ++p) {
        if (is_prime[p]) {
            primes.push_back(p);
        }
    }
    */

    bln_init = true;
}

template<ValidIntegerType T>
Divisors<T>& Divisors<T>::get_instance() {
    static Divisors<T> instance;
    return instance;
}

template<ValidIntegerType T>
void Divisors<T>::set_verbose(bool bln) {
    Globals::verbose = bln;
}

template<ValidIntegerType T>
constexpr int Divisors<T>::__countr_zero(T __x) noexcept
{
    using __gnu_cxx::__int_traits;
    constexpr auto _Nd = __int_traits<T>::__digits;
    if (__x == 0)
        return _Nd;
    constexpr auto _Nd_ull = __int_traits<unsigned long long>::__digits;
    constexpr auto _Nd_ul = __int_traits<unsigned long>::__digits;
    constexpr auto _Nd_u = __int_traits<unsigned>::__digits;
    if (_Nd <= _Nd_u)
        return __builtin_ctz(__x);
    else if (_Nd <= _Nd_ul)
        return __builtin_ctzl(__x);
    else if (_Nd <= _Nd_ull)
        return __builtin_ctzll(__x);
    else 
    {
        static_assert(_Nd <= (2 * _Nd_ull), "Maximum supported integer size is 128-bit");
        constexpr auto __max_ull = __int_traits<unsigned long long>::__max;
        unsigned long long __low = __x & __max_ull;
        if (__low != 0)
            return __builtin_ctzll(__low);
        unsigned long long __high = __x >> _Nd_ull;
        return __builtin_ctzll(__high) + _Nd_ull;
    }
}
    
template<ValidIntegerType T>
int Divisors<T>::bit_scan1(T n) {
    if (n == 0) {
        return -1;
    }
    //return std::countr_zero(n);
    return __countr_zero(n);
}

template<ValidIntegerType T>
int Divisors<T>::bit_length(T n) {
    if (n < 0) {
        return std::bit_width(static_cast<std::make_unsigned_t<T>>(n));
    }
    return std::bit_width(static_cast<std::make_unsigned_t<T>>(n));
}

template<ValidIntegerType T>
T Divisors<T>::integer_pow(T base, int exp) {
    T result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

template<ValidIntegerType T>
std::pair<T, bool> Divisors<T>::iroot(T n, int p) {
    if (n < 0) {
        throw std::invalid_argument("iroot not defined for negative numbers.");
    }
    if (p <= 0) {
        throw std::invalid_argument("p must be a positive integer.");
    }
    if (n == 0 || n == 1) {
        return { n, true };
    }

    T low = 1;
    T high = 2; 
    while (integer_pow(high, p) <= n && high > 0) {
        high *= 2;
    }

    low = high / 2;

    T root = low;

    while (low <= high) {
        T mid = low + (high - low) / 2;
        if (mid == 0) { 
            low = 1;
            continue;
        }

        T mid_pow;
        try {
            mid_pow = integer_pow(mid, p);
        } catch (const std::overflow_error&) {
            high = mid - 1;
            continue;
        }

        if (mid_pow == n) {
            return { mid, true };
        }
        if (mid_pow < n) {
            root = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    //bool exact = integer_pow(root, p) == n;
    bool exact = n % root == 0;

    return { root, exact };
}
    
template<ValidIntegerType T>
std::pair<T, T> Divisors<T>::sqrtrem(T N) {
    std::pair<T, T> tpl = isqrt3b(N, 1, N);
    return { tpl.first, N - tpl.second };
}
        
template<ValidIntegerType T>
T Divisors<T>::isqrt3a(T N) {
    if (N < 0) {
        throw std::invalid_argument("Square root not defined for negative numbers.");
    }
    if (N == 0) {
        return 0;
    }
    return isqrt3b(N, static_cast<T>(1), N).first;
}

template<ValidIntegerType T>
std::pair<T, T> Divisors<T>::isqrt3b(T N, T Low, T High) {
    if (Low >= High) {
        T HighSqr = High * High;
        if (HighSqr > N) {
            High--;
            return { High, High * High };
        } else {
            return { High, HighSqr };
        }
    }

    T Mid = Low + (High - Low) / 2;

    if (Mid > N / Mid) {
        return isqrt3b(N, Low, Mid - 1);
    }

    T MidSqr = Mid * Mid;

    if (MidSqr == N) {
        return { Mid, MidSqr };
    }

    if (MidSqr < N) {
        return isqrt3b(N, Mid + 1, High);
    }

    return isqrt3b(N, Low, Mid - 1);
}
    
    
template<ValidIntegerType T>
T Divisors<T>::isqrt5(T n) {
    if (n < 0) return 0;
    if (n <= 1) return n;

    T result = 0;
    T bit = T(1) << (sizeof(T) * 8 - 2);

    while (bit > n) {
        bit >>= 2;
    }

    while (bit != 0) {
        if (n >= result + bit) {
            n -= result + bit;
            result = (result >> 1) + bit;
        }
        else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}
        
template<ValidIntegerType T>
int Divisors<T>::num_digits(T n) {
    if (n == 0) {
        return 1;
    }
    return static_cast<int>(std::floor(std::log10(static_cast<long double>(std::abs(n))))) + 1;
}

template<ValidIntegerType T>
bool Divisors<T>::is_prime(T n) {
    return setprimes[n];
}
    
template<ValidIntegerType T>
std::pair<T, int> Divisors<T>::remove(T n, T p) {
    int m = 0;
    while (n % p == 0) {
        n /= p;
        m++;
    }
    return { n, m };
}
    
template<ValidIntegerType T>
std::vector<T> Divisors<T>::divisors(T n) {
    T abs_n = n < 0 ? -n : n;
    if (setprimes[n]) {
        return { 1, n };
    }
    if (setprimes[n/2]) {
        return { 1, 2, n/2, n };
    }
    if (setprimes[n/3]) {
        return { 1, 3, n/3, n };
    }
    std::vector<T> result = _divisors(abs_n);
    std::sort(result.begin(), result.end());
    return result;
}

//cannot convert argument 2 from 'std::map<T,int, ...>' to 'const std::map<int64_t,int64_t, ...> &
template<ValidIntegerType T>
std::vector<T> Divisors<T>::_rec_gen(T n, const std::map<T, int>& factors, const std::vector<T>& keys) {
    if (n == static_cast<int64_t>(keys.size())) {
        return { 1 };
    }
    else {
        std::vector<T> pows;
        pows.push_back(1);
        for (int64_t i = 0; i < factors.at(keys[n]); ++i) {
            pows.push_back(pows.back() * keys[n]);
        }
        std::vector<T> next_generation = _rec_gen(n + 1, factors, keys);
        std::vector<T> result;
        for (T q : next_generation) {
            for (T p : pows) {
                result.push_back(p * q);
            }
        }
        return result;
    }
}

template<ValidIntegerType T>
std::vector<T> Divisors<T>::_divisors(T n) {
    if (n == 0) return { 0 };
    if (n == 1) return { 1 };

    const std::map<T, int> factors = factorint(n, 0);

    std::vector<T> ps;
    for (const auto& pair : factors) {
        ps.push_back(pair.first);
    }

    return  _rec_gen(0, factors, ps);
}

template<ValidIntegerType T>
std::pair<T, T> Divisors<T>::_factorint_small(std::map<T, int>& factors, T n, T limit, int fail_max, T next_p) {
    if (Globals::verbose) std::cout << "_factorint_small(factors=" << to_string<T, int>(factors) << ", n = " << n << ", next_p = " << next_p << ")" << std::endl;

    auto done = [&](T current_n, T d) -> std::pair<T, T> {
        if (d * d <= current_n) {
            return { current_n, d };
        }
        return { current_n, 0 };
    };

    T limit2 = limit * limit;
    T threshold2 = std::min(n, limit2);

    if (next_p < 3) {
        if ((n & 1) == 0) {
            auto m = bit_scan1(n);
            factors[2] = m;
            n >>= m;
            threshold2 = std::min(n, limit2);
        }
        next_p = 3;
        if (threshold2 < 9) {
            return done(n, next_p);
        }
    }

    if (next_p < 5) {
        if (n % 3 == 0) {
            n /= 3;
            int m = 1;
            while (n % 3 == 0) {
                n /= 3;
                m++;
                if (m == 20) {
                    auto [nn, mm] = remove(n, 3);
                    n = nn;
                    m += mm;
                    break;
                }
            }
            factors[3] = m;
            threshold2 = std::min(n, limit2);
        }
        next_p = 5;
        if (threshold2 < 25) {
            return done(n, next_p);
        }
    }

    T p6 = next_p % 6;
    next_p += (p6 < 2 ? -1 : 5) - p6;

    int fails = 0;
    while (fails < fail_max) {
        if (n % next_p != 0) {
            fails++;
        }
        else {
            n /= next_p;
            int m = 1;
            while (n % next_p == 0) {
                n /= next_p;
                m++;
                if (m == 20) {
                    auto [nn, mm] = remove(n, next_p);
                    n = nn;
                    m += mm;
                    break;
                }
            }
            factors[next_p] = m;
            fails = 0;
            threshold2 = std::min(n, limit2);
        }
        next_p += 2;
        if (threshold2 < next_p * next_p) {
            return done(n, next_p);
        }

        if (n % next_p != 0) {
            fails++;
        }
        else {
            n /= next_p;
            int m = 1;
            while (n % next_p == 0) {
                n /= next_p;
                m++;
                if (m == 20) {
                    auto [nn, mm] = remove(n, next_p);
                    n = nn;
                    m += mm;
                    break;
                }
            }
            factors[next_p] = m;
            fails = 0;
            threshold2 = std::min(n, limit2);
        }
        next_p += 4;
        if (threshold2 < next_p * next_p) {
            return done(n, next_p);
        }
    }
    return done(n, next_p);
}

template<ValidIntegerType T>
bool Divisors<T>::_check_termination(std::map<T, int>& factors, T n, T next_p, int call_depth) {
    if (Globals::verbose) std::cout << "_check_termination(factors=" << to_string<T, int>(factors) << ", n = " << n << ", next_p = " << next_p << ", call_depth=" << call_depth << ")" << std::endl;

    if (n == 1 || call_depth >= MAX_RECURSION) {
        return true;
    }

    size_t un = static_cast<size_t>(n);

    if (Globals::verbose) std::cout << "_check_termination() n < next_p * next_p (" << n << " < " << next_p << " * " << next_p << ") ? " << (n < next_p * next_p) << std::endl;
    if (Globals::verbose) std::cout << "_check_termination() n < setprimes.size() (" << n << " < " << setprimes.size() << ") ? " << (un < setprimes.size()) << std::endl;
    if (n < next_p * next_p || (un < setprimes.size() && setprimes[n])) {
        factor_cache[n] = n;
        factors[n] = 1;
        return true;
    }

    //T base = 0;
    //int exp = 0;
    auto [base, exp] = _perfect_power(n, next_p, call_depth + 1);
    if (Globals::verbose) std::cout << "_perfect_power(n=" << n << ", next_p=" << next_p << ") returned base=" << base << ", exp=" << exp << std::endl;

    if (base == 0 && exp == 0) {
        return false;
    }

    size_t ubase = static_cast<size_t>(base);

    if (Globals::verbose) std::cout << "_check_termination() base < next_p * next_p (" << base << " < " << next_p << " * " << next_p << ") ? " << (base < next_p * next_p) << std::endl;
    if (Globals::verbose) std::cout << "_check_termination() base < setprimes.size() (" << base << " < " << setprimes.size() << ") ? " << (ubase < setprimes.size()) << std::endl;
    if (base < next_p * next_p || (ubase < setprimes.size() && setprimes[base])) {
        factor_cache[n] = base;
        factors[base] = exp;
    } else {
        std::map<T, int> facs = factorint(base, call_depth + 1);
        for (const auto& pair : facs) {
            factors[pair.first] = exp * pair.second;
        }
    }
    return true;
}


template<ValidIntegerType T>
std::pair<T, bool> Divisors<T>::_trial(std::map<T, int>& factors, T n, const std::vector<T>& candidates) {
    if (Globals::verbose) std::cout << "_trial(factors=" << to_string<T, int>(factors) << ", n = " << n << ", candidates = " << to_string<T>(candidates) << ")" << std::endl;

    auto nfactors_before = factors.size();

    for (const T& d : candidates) {
        if (n % d == 0) {
            if (n != d) {
                factor_cache[n] = d;
            }

            int m = 0;
            while (n % d == 0) {
                n /= d;
                m++;
            }
            factors[d] += m;
        }
    }

    bool factors_changed = factors.size() != nfactors_before;

    return { n, factors_changed };
}

template<ValidIntegerType T>
std::pair<T, int> Divisors<T>::_perfect_power_done(T current_n, std::map<T, int>& current_factors, int current_g, int current_multi) {
    if (current_g == 0) {
        current_g = current_multi;
    } else {
        current_g = std::gcd(current_g, current_multi);
    }

    if (current_g == 1) {
        return std::pair<T, int>(0, 0);
    }

    current_factors[current_n] = current_multi;
    T i = 1;
    for (const auto& pair : current_factors) {
        T p = pair.first;
        int e = pair.second;
        i *= static_cast<T>(pow(p, e / current_g));
    }
    return std::pair<T, int>(i, current_g);
}

template<ValidIntegerType T>
std::pair<T, int> Divisors<T>::_perfect_power(T n, T next_p, int call_depth) {
    if (Globals::verbose) std::cout << "_perfect_power(n=" << n << ", next_p=" << next_p << ", call_depth=" << call_depth << ")" << std::endl;

    if (n <= 3) {
        return { 0, 0 };
    }

    std::map<T, int> factors;
    int g = 0;
    int multi = 1;

    /*
    std::pair<T, int> done = [&](T current_n, std::map<T, int>& current_factors, int current_g, int current_multi) {
        if (current_g == 0) current_g = current_multi;
        else current_g = std::gcd(current_g, current_multi);

        if (current_g == 1) {
            return std::pair<T, int>(0, 0);
        }

        current_factors[current_n] = current_multi;
        T i = 1;
        for (const auto& pair : current_factors) {
            T p = pair.first;
            int e = pair.second;
            i *= static_cast<T>(pow(p, e / current_g));
        }
        return std::pair<T, int>(i, current_g);
    };
    */

    if (n <= 1000000) {
        auto [remaining_n, unused_next_p] = _factorint_small(factors, n, 1000, 1000, next_p);
        if (remaining_n > 1) {
            return std::pair<T, int>(0, 0);
        }

        std::vector<int> factors_values;
        for (const auto& pair : factors) {
            factors_values.push_back(pair.second);
        }

        g = factors_values[0];
        for (size_t i = 1; i < factors_values.size(); ++i) {
            g = std::gcd(g, factors_values[i]);
        }

        if (g == 1) {
            return std::pair<T, int>(0, 0);
        }

        T i = 1;
        for (const auto& pair : factors) {
            T p = pair.first;
            int e = pair.second;
            i *= static_cast<T>(pow(p, e / g));
        }
        return std::pair<T, int>(i, g);
    }

    if (next_p < 3) {
        g = bit_scan1(n);
        if (g != 0) {
            if (g == 1) {
                return std::pair<T, int>(0, 0);
            }
            n >>= g;
            factors[2] = g;
            if (n == 1) {
                return std::pair<T, int>(2, g);
            } else {
                auto [m, exact] = iroot(n, g);
                if (exact) {
                    //return { static_cast<T>(2 * m), g };
                    return std::pair<T, int>(2 * m, g);
                } else if (g < setprimes.size() && setprimes[g]) {
                    return std::pair<T, int>(0, 0);
                }
            }
        }
        next_p = 3;
    }

    while ((n & 7) == 1) {
        auto [m, exact] = iroot(n, 2);
        if (exact) {
            n = m;
            multi <<= 1;
        } else {
            break;
        }
    }
    if (n < static_cast<T>(pow(next_p, 3))) {
        return _perfect_power_done(n, factors, g, multi);
    }

    T tf_max = static_cast<T>(bit_length(n) / 27 + 24);
    if (next_p < tf_max) {
        for (T p = next_p; p < tf_max; p += 2) {
            if (p < setprimes.size() && setprimes[p]) {
                auto [m_n, t] = remove(n, p);
                if (t != 0) {
                    n = m_n;
                    t *= multi;
                    int _g = std::gcd(g, t);
                    if (_g == 1) {
                        return std::pair<T, int>(0, 0);
                    }
                    factors[p] = t;
                    if (n == 1) {
                        T i = 1;
                        for (const auto& pair : factors) {
                            i *= static_cast<T>(pow(pair.first, pair.second / _g));
                        }
                        return std::pair<T, int>(i, _g);
                    } else if (g == 0 || _g < g) {
                        g = _g;
                        auto [m, exact] = iroot(static_cast<T>(pow(n, multi)), g);
                        if (exact) {
                            T i = m;
                            for (const auto& pair : factors) {
                                i *= static_cast<T>(pow(pair.first, pair.second / g));
                            }
                            return std::pair<T, int>(i, g);
                        } else if (g < setprimes.size() && setprimes[g]) {
                            return std::pair<T, int>(0, 0);
                        }
                    }
                }
            }
        }
        next_p = tf_max;
    }
    if (n < static_cast<T>(pow(next_p, 3))) {
        return _perfect_power_done(n, factors, g, multi);
    }
    return _perfect_power_done(n, factors, g, multi);
}

template<ValidIntegerType T>
size_t Divisors<T>::find(T p) {
    auto it = idxprimes.find(p);
    if (it != idxprimes.end()) {
        return it->second;
    }
    auto it_upper = std::upper_bound(aryprimes.begin(), aryprimes.end(), p);
    if (it_upper != aryprimes.end()) {
        int idx_upper= std::distance(aryprimes.begin(), it_upper);
        idxprimes[p] = idx_upper;
        return idx_upper;
    }
    return -1;
}

template<ValidIntegerType T>
std::map<T, int> Divisors<T>::factorint(T n, int call_depth) {
    if (Globals::verbose) std::cout << "factorint(n=" << n << ", call_depth=" << call_depth << ")" << std::endl;

    bool use_trial = true;
    bool use_rho = true;
    bool use_pm1 = true;
    bool use_ecm = false;

    if (n < 0) {
        std::map<T, int> factors = factorint(abs(n), call_depth + 1);
        factors[-1] = 1;
        return factors;
    }

    if (n < 100) {
        return small_factor_cache[n];
    }

    std::map<T, int> factors;
    if (call_depth >= MAX_RECURSION) {
        return factors;
    }

    T small = 2 << 14; // 2**15 equivalent.
    int fail_max = 600;

    auto [remaining_n, next_p] = _factorint_small(factors, n, small, fail_max, 2);
    n = remaining_n;

    if (next_p == 0) {
        if (n > 1) {
            factors[n] = 1;
        }
        return factors;
    }

    while (factor_cache.count(n)) {
        T p = factor_cache[n];
        auto [new_n, e] = remove(n, p);
        n = new_n;
        factors[p] = factors.count(p) ? factors[p] + e : e;
    }

    bool check_term = _check_termination(factors, n, next_p, call_depth + 1);
    if (Globals::verbose) std::cout << "_check_termination(" << to_string<T, int>(factors) << ", " << n << ", " << next_p << ") returned " << check_term << std::endl;
    if (check_term) {
        return factors;
    }

    T sqrt_n = isqrt3a(n);
    T a = sqrt_n + 1;
    if ((n % 4 == 1) ^ ((a & 1) != 0)) {
        a += 1;
    }
    T a2 = a * a;
    T b2 = a2 - n;

    for (int _ = 0; _ < 3; ++_) {
        auto [b, fermat] = sqrtrem(b2);
        if (!fermat) {
            for (T r : {a - b, a + b}) {
                std::map<T, int> facs = factorint(r, call_depth + 1);
                for (const auto& pair : facs) {
                    factors[pair.first] += pair.second;
                }
            }
            return factors;
        }
        b2 += (a + 1) * 4;
        a += 2;
    }

    T low = next_p;
    T high = 2 * next_p;
    T _limit = sqrt_n + 1;
    int iteration = 0;
    bool found_trial = false;

    while (true) {
        T high_ = std::min(high, _limit);
        if (use_trial) {
            std::vector<long long> ps;
            size_t idx_low = find(low);
            size_t idx_high = find(high_);
            if (idx_low > -1 && idx_high > -1) {
                //auto end_it = (idx_high > 0 && idx_high <= aryprimes.size()) ? aryprimes.begin() + idx_high : aryprimes.end();
                ps = std::vector<long long>(aryprimes.begin() + idx_low, (idx_high > 0) ? aryprimes.begin() + idx_high : aryprimes.end());
            } else {
                for (int p = low; p < high_; p += 2) {
                    if (setprimes[p]) {
                        ps.push_back(p);
                    }
                }
            }
            T new_n = 0;
            std::tie(new_n, found_trial) = _trial(factors, n, ps);
            if (Globals::verbose) std::cout << "_trial(n=" << n << ", ps=primerange(" << low << ", " << high_ << ")) returned " << new_n << ", " << found_trial << std::endl;
            n = new_n;
            next_p = high_;
            if (found_trial) {
                bool check_term = _check_termination(factors, n, next_p, call_depth + 1);
                if (Globals::verbose) std::cout << "_check_termination(" << to_string<T, int>(factors) << ", " << n << ", " << next_p << ") returned " << check_term << std::endl;
                if (check_term) {
                    return factors;
                }
            }
        }

        if (high > _limit) {
            if (n > 1) {
                factors[n] = factors.count(n) ? factors[n] + 1 : 1;
            }
            return factors;
        }

        if (!found_trial) {
            if (use_pm1) {
                long long c = pollard_pm1(n, low, high_);
                if (Globals::verbose) std::cout << "pollard_pm1(n=" << n << ", B=" << low << ", seed=" << high_ << ") returned c=" << c << std::endl;
                if (c) {
                    std::vector<long long> ps;
                    if (c < next_p * next_p || setprimes[c]) {
                        ps.push_back(c);
                    } else {
                        std::map<long long, int> factors_map = factorint(c, call_depth + 1);
                        for (const auto& pair : factors_map) {
                            ps.push_back(pair.first);
                        }
                    }
                    bool b = false;
                    std::tie(n, b) = _trial(factors, n, ps);
                    bool check_term = _check_termination(factors, n, next_p, call_depth + 1);
                    if (Globals::verbose) std::cout << "_check_termination(" << to_string<T, int>(factors) << ", " << n << ", " << next_p << ") returned " << check_term << std::endl;
                    if (check_term) {
                        return factors;
                    }
                }
            }

            if (use_rho) {
                long long c = pollard_rho(n, 1, low, high_);
                if (Globals::verbose) std::cout << "pollard_rho(n=" << n << ", retries=1, max_steps=" << low << ", seed=" << high_ << ") returned c = " << c << std::endl;
                if (c) {
                    std::vector<long long> ps;
                    if (c < next_p * next_p || setprimes[c]) {
                        ps.push_back(c);
                    } else {
                        std::map<long long, int> factors_map = factorint(c, call_depth + 1);
                        for (const auto& pair : factors_map) {
                            ps.push_back(pair.first);
                        }
                    }
                    bool b = false;
                    std::tie(n, b) = _trial(factors, n, ps);
                    bool check_term = _check_termination(factors, n, next_p, call_depth + 1);
                    if (check_term) {
                        return factors;
                    }
                }
            }
        }

        iteration++;
        if (use_ecm && iteration >= 3 && num_digits(n) >= 24) {
            break;
        }

        low = high;
        high *= 2;
    }

    // ECM loop (commented out in Python, so optional here)
    // T B1 = 10000;
    // T B2 = 100 * B1;
    // int num_curves = 50;
    // while(true) {
    //     T factor = _ecm_one_factor(n, B1, B2, num_curves, B1);
    //     if (factor != 0) {
    //         // ... (rest of ECM logic)
    //     }
    //     B1 *= 5;
    //     B2 = 100 * B1;
    //     num_curves *= 4;
    // }

    return factors;
}


template<ValidIntegerType T>
//T Divisors<T>::pollard_pm1(T n, int B = 10, T a = 2, int retries = 0, unsigned int seed = 1234) {
T Divisors<T>::pollard_pm1(T n, int B, T a, int retries, unsigned int seed) {
    if (Globals::verbose) std::cout << "pollard_pm1(n=" << n << ", B=" << B << ", a=" << a << ", retries=" << retries << ", seed=" << seed << ")" << std::endl;

    if (n < 4 || B < 3) {
        throw std::invalid_argument("pollard_pm1 should receive n > 3 and B > 2");
    }

    std::mt19937_64 rng(seed + B);
    std::uniform_int_distribution<T> dist(2, n - 2);

    for (int i = 0; i < retries + 1; ++i) {
        T aM = a;

        for (int p = 2; p <= B; ++p) {
            if (p == 2 || (p > 2 && p % 2 != 0 && p < setprimes.size() && setprimes[p])) {
                int e = static_cast<int>(log(B) / log(p));
                long long power_pe = 1;
                for (int j = 0; j < e; ++j) {
                    power_pe *= p;
                }
                aM = std::pow(aM, power_pe);
                aM = aM % n;
            }
        }

        T g = std::gcd(aM - 1, n);

        if (g > 1 && g < n) {
            return g;
        }

        a = dist(rng);
    }
    return 0;
}

template<ValidIntegerType T>
T Divisors<T>::power_mod(T base, T exp, T mod) {
    T res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

template<ValidIntegerType T>
T Divisors<T>::pollard_rho_f(T x, T n2, T a2) {
    T x_squared_mod = (x * x) % n2;
    return (x_squared_mod + a2) % n2;
}

template<ValidIntegerType T>
//T Divisors<T>::pollard_rho(T n1, T s = 2, T a1 = 1, int retries = 5, unsigned int seed = 1234, int64_t max_steps = 0) {
T Divisors<T>::pollard_rho(T n1, T s, T a1, int retries, unsigned int seed, int64_t max_steps) {
    if (Globals::verbose) std::cout << "pollard_rho(n1=" << n1 << ", s=" << s << ", a1= " << a1 << ", retries=" << retries << ", seed=" << seed << ", max_steps=" << max_steps << ")" << std::endl;

    if (n1 < 5) {
        throw std::invalid_argument("pollard_rho should receive n > 4");
    }

    std::mt19937_64 rng(seed + retries);
    std::uniform_int_distribution<long long> dist_v(0, n1 - 2);
    std::uniform_int_distribution<long long> dist_a(1, n1 - 4);

    T V = s;

    for (int i = 0; i < retries + 1; ++i) {
        T U = V;
        T j = 0;

        while (true) {
            if (max_steps > 0 && j > max_steps) {
                break;
            }
            j++;

            U = pollard_rho_f(U, n1, a1);
            V = pollard_rho_f(pollard_rho_f(V, n1, a1), n1, a1);

            T diff = U - V;
            if (diff < 0) {
                diff += n1;
            }
            T g = std::gcd(diff, n1);

            if (g == 1) {
                continue;
            }
            if (g == n1) {
                break;
            }
            return g;
        }

        V = dist_v(rng);
        a1 = dist_a(rng);
    }

    return 0;
}



std::vector<int64_t> divisors(int n) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    return div.divisors(n);
}

std::pair<int64_t, int> _factorint_small(int n, int p) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    std::map<int64_t, int> factors;
    auto [remaining_n, next_p] = div._factorint_small(factors, n, 2 << 14, 600, p);
    return { remaining_n, next_p };
}

std::pair<int64_t, int> _perfect_power(int n, int p) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    return div._perfect_power(n, p, 0);
}

//std::pair<T, bool> _trial(std::map<T, int>& factors, T n, const std::vector<T>& candidates) {
std::pair<int64_t, bool> _trial(std::map<int64_t, int>& factors, int64_t n, const std::vector<int64_t>& candidates) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    return div._trial(factors, n, candidates);
}

//_check_termination(std::map<T, int>& factors, T n, T next_p, int call_depth) {
bool _check_termination(std::map<int64_t, int>& factors, int64_t n, int64_t next_p) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    return div._check_termination(factors, n, next_p, 1);
}

//pollard_pm1(n, B=low, seed=high_)
//pollard_pm1(T n, int B = 10, T a = 2, int retries = 0, unsigned int seed = 1234) 
int64_t pollard_pm1(int64_t n, int B, unsigned int seed) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    return div.pollard_pm1(n, B, 2, 0, seed);
}

//pollard_rho(n, retries=1, max_steps=low, seed=high_)
//pollard_rho(T n1, T s = 2, T a1 = 1, int retries = 5, unsigned int seed = 1234, uint64_t max_steps = 0)
int64_t pollard_rho(int64_t n, uint64_t max_steps, unsigned int seed) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    return div.pollard_rho(n, 2, 1, 1, seed, max_steps);
}

int64_t isqrt(int n) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    return div.isqrt3a(n);
}

std::pair<int64_t, int64_t> sqrtrem(int n) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    return div.sqrtrem(n);
}

bool is_prime(int n) {
    Divisors<int64_t> div = Divisors<int64_t>::get_instance();
    return div.is_prime(n);
}

void set_verbose(bool v) {
    Globals::verbose = v;
}


/*

git remote add origin https://aweslowski+Cpp@gmail.com@github.com/repository_owner/repository_name.git
git remote add origin https://aweslowski%2BCpp%40gmail%2Ecom@github.com/AlexWeslowski%2DCpp/Divisors.git

 git remote set-url origin https://aweslowski%2BCpp%40gmail%2Ecom@github.com/AlexWeslowski%2DCpp/Divisors.git
 git remote set-url origin https://AlexWeslowski@github.com/AlexWeslowski/Divisors.git


Scripts\pip.exe install C:\Users\alex.weslowski\Documents\C++\Divisors

import sympy
import sympy.external.gmpy
import divisors
import numba
import numpy
import math
import time
import random
import sys
import os
import importlib
sys.path.append('C:\\Users\\alex.weslowski\\Documents\\Python\\Sequence')
import sequence
import sympy_ntheory_factor

importlib.reload(sequence)
importlib.reload(sympy_ntheory_factor)

# 2^24 = 16777216 = 2.12-3.27 seconds
t1 = time.time()
ap = 2**24
sequence.setprimes = numpy.array([False]*(ap+1), dtype=bool)
for p in sympy.sieve.primerange(ap+1):
    sequence.setprimes[p] = True
dt = time.time() - t1

# 2^24 = 16777216 = 0.005-0.007 seconds
t1 = time.time()
div = divisors.Divisors()
div = divisors.Divisors.get_instance()
div.is_prime(11)
div.is_prime(13033927)
dt = time.time() - t1

def print_time(func_name, len_ary, dt):
    if dt < 60:
        print(f"{func_name} {len_ary:,} in {round(dt, 2):.2f} seconds")
    else:
        print(f"{func_name} {len_ary:,} in {round(dt/60, 2):.2f} minutes")



def time_sequence_divisors(ary):
    t1 = time.time()
    for i in range(0, len(ary)):
        sd = sequence.divisors(ary[i], sequence.small_factor_cache, sequence.factor_cache)
    print_time("sequence.divisors()", len(ary), time.time() - t1)

def time_sympy_ntheory_factor_divisors(ary):
    t1 = time.time()
    for i in range(0, len(ary)):
        sd = sympy_ntheory_factor.divisors(ary[i])
    print_time("sympy_ntheory_factor.divisors()", len(ary), time.time() - t1)

def time_sympy_ntheory_divisors(ary):
    t1 = time.time()
    for i in range(0, len(ary)):
        sd = sympy.ntheory.divisors(ary[i])
    print_time("sympy.ntheory.divisors()", len(ary), time.time() - t1)

def time_divisors(ary):
    div = divisors.Divisors.get_instance()
    t1 = time.time()
    for i in range(0, len(ary)):
        sd = div.divisors(ary[i])
    print_time("Divisors.divisors()", len(ary), time.time() - t1)

def compare(ary, verbose=False):
    icount = 0
    hsh = {
    "sympy.ntheory.divisors()":0.0, 
    "sympy_ntheory_factor.divisors()":0.0,
    "sequence.divisors()":0.0, 
    "divisors.divisors()":0.0
    }
    for a in ary:
        symd = []
        snfd = []
        seqd = []
        divd = []

        if True:
            key = "sympy.ntheory.divisors()"
            if verbose: print(key)
            t1 = time.time()
            symd = sympy.ntheory.divisors(a)
            hsh[key] += (time.time() - t1)
        
        if True:
            key = "sympy_ntheory_factor.divisors()"
            if verbose: print(key)
            t1 = time.time()
            snfd = sympy_ntheory_factor.divisors(a, sequence.small_factor_cache, sequence.factor_cache)
            hsh[key] += (time.time() - t1)
        
        if False:
            key = "sequence.divisors()"
            if verbose: print(key)
            t1 = time.time()
            seqd = sequence.divisors(a, sequence.small_factor_cache, sequence.factor_cache)
            hsh[key] += (time.time() - t1)
        
        if True:
            key = "divisors.divisors()"
            if verbose: print(key)
            t1 = time.time()
            divd = divisors.divisors(a)
            hsh[key] += (time.time() - t1)

        icount += 1
        if symd != divd:
            print(f"symd = {symd}")
            print(f"snfd = {snfd}")
            #print(f"seqd = {seqd}")
            print(f"divd = {divd}")
            print(f"n = {a}")
            return False, icount, hsh
    return True, icount, hsh

#symd = [1, 3643, 4261, 15522823]
#seqd = [1, 3643, 4261, 15522823]
#divd = [1, 15522823]
#n = 15522823
#n = 15458581
ary = [random.randint(2**12, 2**24) for i in range(0, 2**20)]
compare(ary)

1048576
sympy.ntheory.divisors(): 47.76-64.52
sympy_ntheory_factor.divisors(): 43.54-44.44
divisors.divisors(): 5.74-6.18

sequence.verbose = False
time_sequence_divisors(ary)
time_sympy_ntheory_divisors(ary)
sympy_ntheory_factor.debug = False
time_sympy_ntheory_factor_divisors(ary)
divisors.set_verbose(False)
time_divisors(ary)

n = 15458581
n = 13033927 # [1, 13033927]
n = 16050178 # [1, 2, 8025089, 16050178]
n = 16675619

factors = numba.typed.Dict.empty(key_type=numba.int64, value_type=numba.int64)
sequence.divisors(16056497, sequence.small_factor_cache, sequence.factor_cache)
sequence._check_termination(factors, 16056497, 1805)

divisors._check_termination({2:1}, 8025089, 1805)
divisors._check_termination(dict(), 16675619, 1805)
divisors._check_termination(dict(), 16056497, 1805)

symd = sympy_ntheory_factor.divisors(n)
#symd = sympy.divisors(n)
#symd = sympy.ntheory.divisors(n)
seqd = sequence.divisors(n, sequence.small_factor_cache, sequence.factor_cache)
divd = divisors.divisors(n)

ps = [x for x in range(1811, 2733, 2) if sequence.setprimes[x]]
factors = numba.typed.Dict.empty(key_type=numba.int64, value_type=numba.int64)
factors[2] = 1
sympy_ntheory_factor._trial(factors, 7477033, ps)
sequence._trial(factors, 7477033, ps, sequence.factor_cache)
divisors._trial({2:1}, 7477033, ps)

sympy.external.gmpy.sqrtrem(3192)
sequence.sqrtrem(3192)
divisors.sqrtrem(3192)

symfact = sympy.factorint(n)
divfact = divisors.factorint(n)
seqfact = sequence.factorint(n, sequence.small_factor_cache, sequence.factor_cache, 0)

factors = numba.typed.Dict.empty(key_type=numba.int64, value_type=numba.int64)
divisors._factorint_small(9671029)
sequence._factorint_small(factors, 9671029, 2**15, 600, 2)


sympy.factorint(n, limit=None, use_trial=True, use_rho=True, use_pm1=True, use_ecm=False, verbose=False, visual=None, multiple=False)
sequence.factorint(n, sequence.small_factor_cache, sequence.factor_cache, 0)

n = 1
symtpl = (0, 0)
seqtpl = (0, 0)
divtpl = (0, 0)
setprimes = [x for x in range(10**3, 2**24) if sequence.setprimes[x]]
for i in range(0, 2**18):
    n = random.randint(10**6, 10**8)
    p = setprimes[random.randint(0, len(setprimes)-1)]
    factors = numba.typed.Dict.empty(key_type=numba.int64, value_type=numba.int64)
    if False:
        symtpl = sympy.ntheory.factor_._factorint_small(factors, n, 2**15, 600, p)
        seqtpl = sequence._factorint_small(factors, n, 2**15, 600, p)
        divtpl = divisors._factorint_small(n, p)
    if True:
        #Not equal for n, p = 44665720, 7471157
        symtpl = sympy.ntheory.factor_._perfect_power(n, p)
        if not symtpl:
            symtpl = (0, 0)
        #_perfect_power(n, small_factor_cache, factor_cache, next_p, call_depth)
        seqtpl = sequence._perfect_power(n, sequence.small_factor_cache, sequence.factor_cache, p, 0)
        divtpl = divisors._perfect_power(n, p)
    if symtpl != seqtpl or symtpl != divtpl or seqtpl != divtpl:
        print(f"Not equal for n={n}, p={p}")
        break

*/

/*
* 
* Scripts\pip.exe install C:\Users\alex.weslowski\Documents\C++\Divisors
* python.exe
* import sympy
* import divisors
* import time
* import random
* 
* sd = sympy.divisors(23*719*3089)
* dd = divisors.divisors(23*719*3089)
* sd == dd
* 
*/


PYBIND11_MODULE(divisors, m) {
    m.doc() = "divisors made with pybind11";

    m.def("divisors", &divisors);
    m.def("is_prime", &is_prime);
    m.def("set_verbose", &set_verbose);

    py::class_<Divisors<int64_t>>(m, "Divisors")
        //.def(py::init<const std::string&>(), py::arg("name"))
        .def("get_instance", &Divisors<int64_t>::get_instance)
        .def("divisors", &Divisors<int64_t>::divisors)
        .def("set_verbose", &Divisors<int64_t>::set_verbose);

    //py::class_<ArrayArray<int64_t>>(m, "ArrayArray");

    /*
    using ArrayArrayT = ArrayArray<int64_t>;
    std::string type_name = "";
    py::class_<ArrayArrayT>(m, ("ArrayArray" + type_name).c_str())
        .def(py::init<size_t, bool>(), py::arg("capacity"), py::arg("resizeable"))
        .def("append", &ArrayArrayT::append, py::arg("ary"))
        .def("removeAt", &ArrayArrayT::removeAt, py::arg("idx"))
        .def("__len__", &ArrayArrayT::getSize)
        .def("__getitem__", &ArrayArrayT::getItem, py::arg("idx"))
        .def("to_array", &ArrayArrayT::to_array)
        .def_readwrite("keys", &ArrayArrayT::keys)
        .def_readwrite("values", &ArrayArrayT::values);
    */

    py::class_<ArrayArray<int64_t>, std::shared_ptr<ArrayArray<int64_t>>>(m, "ArrayArray")
        .def(py::init<size_t, bool>(), py::arg("capacity"), py::arg("resizeable"))
        .def("append", &ArrayArray<int64_t>::append)
        .def("push_back", &ArrayArray<int64_t>::push_back)
        .def("removeAt", &ArrayArray<int64_t>::removeAt)
        .def("to_array", &ArrayArray<int64_t>::to_array)
        //.def("get_size", &ArrayArray<int64_t>::getSize) 
        //.def("get_item", &ArrayArray<int64_t>::getItem) 
        .def("__len__", &ArrayArray<int64_t>::getSize)
        .def("__getitem__", &ArrayArray<int64_t>::getItem);

    py::class_<Combinations<int64_t>>(m, "Combinations")
        .def(py::init())
        .def("backtrack", &Combinations<int64_t>::backtrack)
        .def("set_verbose", &Combinations<int64_t>::set_verbose);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

/*
PYBIND11_MODULE(divisors, m) {
    m.def("divisors", &divisors);
    m.def("_factorint_small", &_factorint_small);
    m.def("_perfect_power", &_perfect_power);
    m.def("_trial", &_trial);
    m.def("_check_termination", &_check_termination);
    m.def("pollard_pm1", &pollard_pm1);
    m.def("pollard_rho", &pollard_rho);
    m.def("isqrt", &isqrt);
    m.def("sqrtrem", &sqrtrem);
    m.def("is_prime", &is_prime);
    m.def("set_verbose", &set_verbose);
}
*/

