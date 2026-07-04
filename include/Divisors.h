// Divisors.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <algorithm>
#include <bit>
#include <bitset>
#include <cctype>
#include <cerrno>
#include <charconv>
#include <csignal>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <mutex>
#include <numeric> 
#include <random>
#include <source_location>
#include <span>
//#include <stacktrace>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <../include/boost/container/small_vector.hpp>
#include <../include/boost/container/flat_map.hpp>
#include <../include/boost/dynamic_bitset.hpp>
#include <../include/boost/integer/common_factor_rt.hpp>
#include <../include/boost/rational.hpp>
//#include <../include/cpp_fractions_lib/Fraction.h>
#include <../include/rational/rational.h>
#include <Python.h>
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <../include/ValidInteger.h>
#include <../include/Builtins.h>
#include <../include/Functions.h>
#include <../include/Globals.h>
#include <../include/ArrayArray.h>
#include <../include/AtomicBitset.h>
#include <../include/AtomicBitReference.h>
#include <../include/primesieve.h>
#include <../include/primesieve.hpp>

#ifndef DIVISORS_H
#define DIVISORS_H

//using Rational = Fraction<T>;
template <typename T>
using BoostRational = boost::rational<T>;
//template <typename T>
//using Rational = Commons::Math::Rational<T>;

#define DIVISORS_SMALLVEC
#define DIVISORS_SMALLMAP
#undef FACTOR_CACHE
#undef SMALLVEC
#undef SMALLMAP

static bool bln_init = false;
// 2^24 =  16777216
// 2^25 =  33554432
// 2^26 =  67108864
// 2^27 = 134217728
// 2^28 = 268435456 // 33554432 bytes // 33.55 MB
const int LEN_SET_PRIMES = 268435456;
const int LEN_DYN_PRIMES = 134217728;
const size_t CANDIDATES_VEC_LEN = 32;
const int MAX_RECURSION = 12;

#ifdef DIVISORS_SMALLMAP
	using map_int64t_int = boost::container::flat_map<int64_t, int>;
	using map_longlong_int = boost::container::flat_map<long long, int>;
#else
	using map_int64t_int = std::map<int64_t, int>;
	using map_longlong_int = std::map<long long, int>;
#endif


// (5)
#ifdef DIVISORS_SMALLVEC
	using vec_primes = boost::container::small_vector<int64_t, 3072>;
	PYBIND11_MAKE_OPAQUE(boost::container::small_vector<int64_t, 3072>);
#else
	using vec_primes = std::vector<int64_t>;
#endif

// (1)
#ifdef DIVISORS_SMALLVEC
	using vec_small_factors = boost::container::small_vector<map_int64t_int, 100>;
#else
	using vec_small_factors = std::vector<std::map<int64_t, int>>;
#endif

template <typename T>
#ifdef DIVISORS_SMALLVEC
	using vec_factors = boost::container::small_vector<T, FACTORS_VEC_LEN>;
	PYBIND11_MAKE_OPAQUE(boost::container::small_vector<int64_t, FACTORS_VEC_LEN>);
#else
	using vec_factors = std::vector<T>;
#endif

// (2)
template <typename T>
#ifdef DIVISORS_SMALLVEC
	using vec_candidates = boost::container::small_vector<T, CANDIDATES_VEC_LEN>;
#else
	using vec_candidates = std::vector<T>;
#endif

// (3)
#ifdef DIVISORS_SMALLVEC
	using vec_ps = boost::container::small_vector<long long, CANDIDATES_VEC_LEN>;
#else
	using vec_ps = std::vector<long long>;
#endif

// (4)
template <typename T>
#ifdef DIVISORS_SMALLVEC
	using vec_divisors = boost::container::small_vector<T, DIVISORS_VEC_LEN>;
	PYBIND11_MAKE_OPAQUE(boost::container::small_vector<int64_t, DIVISORS_VEC_LEN>);
#else
	using vec_divisors = std::vector<T>;
#endif

// 
template <typename T1, typename T2>
#ifdef DIVISORS_SMALLMAP
	using map_t_int = boost::container::flat_map<T1, T2>;
#else
	using map_t_int = std::map<T1, T2>;
#endif

static vec_small_factors small_factor_cache(100);
//static ArrayArray<int64_t, 40134, 201902> divisors_cache;
//static ArrayArray<int64_t, 49152, 245760> divisors_cache;
//static ArrayArray<int64_t, 65536, 327680> divisors_cache;
static ArrayArray<int64_t, DIVISORS_CACHE_KEYS_LEN, DIVISORS_CACHE_VALUES_LEN> static_divisors_cache;
static map_int64t_int static_factor_cache;
static std::bitset<LEN_SET_PRIMES/2 + 1> staticsetprimes;
static boost::dynamic_bitset<> staticdynprimes(LEN_DYN_PRIMES/2 + 1);

static vec_primes aryprimes;
// 
#ifdef DIVISORS_SMALLMAP
	static boost::container::flat_map<int64_t, size_t> idxprimes;
#else
	static std::map<int64_t, size_t> idxprimes;
#endif


/*

#if defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>
#elif defined(__linux__)
#include <fstream>
#include <string>
#endif
#endif

size_t get_current_rss() {
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return (size_t)pmc.WorkingSetSize;
    }
    return 0;
#elif defined(__APPLE__) && defined(__MACH__)
    struct mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS) {
        return (size_t)info.resident_size;
    }
    return 0;
#elif defined(__linux__)
    long rss = 0L;
    std::ifstream fp("/proc/self/statm");
    if (fp.is_open()) {
        fp >> rss; // Skip first field (size)
        fp >> rss; // Second field is resident set size
        return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);
    }
    return 0;
#else
    return 0;
#endif
}
*/

std::string format_with_commas(uint64_t value);
unsigned long long to_long_long(const std::string& str);
std::pair<unsigned long, unsigned long long> thread_id();
uint64_t get_id(bool bln_thread_local = true);

template<ValidIntegerType T>
class Divisors {
private:
    /*
    static std::vector<boost::container::flat_map<T, int>> small_factor_cache;
    static boost::container::flat_map<T, int> factor_cache;
    static boost::container::flat_map<T, size_t> idxprimes;
    */
#ifdef FACTOR_CACHE
	map_int64t_int factor_cache;
#endif
	std::bitset<LEN_SET_PRIMES/2 + 1> setprimes;
    boost::dynamic_bitset<> dynprimes{LEN_DYN_PRIMES/2 + 1}; 
	static unsigned short new_id();

public:
	ArrayArray<int64_t, DIVISORS_CACHE_KEYS_LEN, DIVISORS_CACHE_VALUES_LEN> divisors_cache;
	
	static std::mutex ids_mtx;
	static boost::container::small_vector<unsigned short, 32> ids;
	static boost::container::small_vector<unsigned short, 32> get_ids();
	unsigned short id;
	uint64_t get_id();
    Divisors();
    void init_primes(uint64_t a, uint64_t b);
    bool resize(int64_t n);
    static Divisors<T>& get_instance(int64_t n, bool bln_thread_local);
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
	
	//T mult(const std::span<T>& ary);
	template<size_t N>
	T mult(const std::array<T, N>& ary);
	BoostRational<T> calc_density_bitmask(T i, const vector<T>& a, const BoostRational<T>& max_sum);
	BoostRational<T> calc_density_unrolled(T i, const vector<T>& a, const BoostRational<T>& max_sum);
	uint64_t size();
	std::pair<unsigned long, unsigned long long> thread_id();
	
	vec_divisors<T> divisors(T n);
    vec_divisors<T> _rec_gen(T n, const map_t_int<T, int>& factors, const vec_divisors<T>& keys);
    vec_divisors<T> _divisors(T n);
    std::pair<T, T> _factorint_small(map_t_int<T, int>& factors, T n, T limit, int fail_max, T next_p);
    bool _check_termination(map_t_int<T, int>& factors, T n, T next_p, int call_depth);
    std::pair<T, bool> _trial(map_t_int<T, int>& factors, T n, const vec_ps& candidates);
    std::pair<T, int> _perfect_power_done(T current_n, map_t_int<T, int>& current_factors, int current_g, int current_multi);
    std::pair<T, int> _perfect_power(T n, T next_p, int call_depth);
    size_t find(T p);
    map_t_int<T, int> factorint(T n, int call_depth);
    T pollard_pm1(T n, int B = 10, T a = 2, int retries = 0, unsigned int seed = 1234);
    T power_mod(T base, T exp, T mod);
    T pollard_rho_f(T x, T n2, T a2);
    T pollard_rho(T n1, T s = 2, T a1 = 1, int retries = 5, unsigned int seed = 1234, int64_t max_steps = 0);
};


//template<ValidIntegerType T>
//std::vector<std::map<int64_t, int>> Divisors<T>::small_factor_cache(100);

template<ValidIntegerType T>
std::mutex Divisors<T>::ids_mtx;

template<ValidIntegerType T>
boost::container::small_vector<unsigned short, 32> Divisors<T>::ids;

#endif
