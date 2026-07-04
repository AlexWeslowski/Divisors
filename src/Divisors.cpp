#include "../include/ArrayArray.h"
#include "../include/Divisors.h"
#include <../include/Combinations.h>
#include <../include/CombinationsIterator.h>

using namespace std;
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


/*
void signal_handler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received" << std::endl;
    //std::cout << boost::stacktrace::stacktrace() << std::endl;
	std::cout << std::stacktrace::current() << std::endl;
    std::exit(signum);
}
*/

std::string format_with_commas(uint64_t value) {
    std::string num_str = std::to_string(value);
    size_t insert_position = num_str.length() - 3;
    while (insert_position > 0) {
        num_str.insert(insert_position, ",");
        insert_position -= 3;
    }
    return num_str;
}

unsigned long long to_long_long(const std::string& str) {
    if (str.empty()) {
		return 0;
	}
	auto hash = [&str]() -> unsigned long long {
        std::hash<std::string> hasher;
        return hasher(str);
    };
    std::string s = str;    
    bool is_hex = false;
    size_t start_idx = 0;
    if (s[0] == '-' || s[0] == '+') {
        start_idx = 1;
    }
    if (s.size() > start_idx + 2 && s[start_idx] == '0' && (s[start_idx + 1] == 'x' || s[start_idx + 1] == 'X')) {
        is_hex = true;
        start_idx += 2;
    }
    if (start_idx == s.size()) {
		return hash();
	}
    for (size_t i = start_idx; i < s.size(); ++i) {
        if (is_hex) {
            if (!std::isxdigit(static_cast<unsigned char>(s[i]))) return 0;
        } else {
            if (!std::isdigit(static_cast<unsigned char>(s[i]))) return 0;
        }
    }
    try {
        int base = is_hex ? 16 : 10;
        return std::stoull(s, nullptr, base);
    } catch (const std::exception&) {
        return hash();
    }
}

uint64_t get_id(bool bln_thread_local) {
	Divisors<int64_t>& div = Divisors<int64_t>::get_instance(LEN_SET_PRIMES, bln_thread_local);
	return div.get_id();
}

std::pair<unsigned long, unsigned long long> thread_id() {
	std::string id = std::format("{}", std::this_thread::get_id());
	return std::make_pair(PyThread_get_thread_ident(), to_long_long(id));
}

template<ValidIntegerType T>
unsigned short Divisors<T>::new_id() {
	std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned short> distrib(1000, 9999);
	unsigned short id = distrib(gen);
	
	std::lock_guard<std::mutex> lock(ids_mtx);
	while (std::ranges::contains(ids, id)) {
		id = distrib(gen);
	}
	ids.push_back(id);
	return id;
}

template<ValidIntegerType T>
uint64_t Divisors<T>::get_id() {
	return id;
}

template<ValidIntegerType T>
boost::container::small_vector<unsigned short, 32> Divisors<T>::get_ids() {
	return ids;
}

template<ValidIntegerType T>
Divisors<T>::Divisors() {
	/*
    if (bln_init) {
        return;
    }
	*/
	id = new_id();
	
	if (Globals::verbose) {
		std::cout << "Divisors() bln_init = " << bln_init << std::endl;
		std::cout << "Divisors() setprimes.size() = " << setprimes.size() << std::endl;
#ifdef DIVISORS_SMALLVEC
		std::cout << "Divisors() DIVISORS_SMALLVEC" << std::endl;
#endif
#ifdef SMALL
		std::cout << "Divisors() SMALL" << std::endl;
#endif
#ifdef SMALLVEC
		std::cout << "Divisors() SMALLVEC" << std::endl;
#endif
	}
    //std::signal(SIGSEGV, signal_handler);
    //std::signal(SIGABRT, signal_handler);
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
	
	small_factor_cache.resize(100);
	/*
	for (int i = 0; i < 100; ++i) {
        small_factor_cache.emplace_back(); 
    }
	*/
	if (Globals::verbose) std::cout << "Divisors() line " << __LINE__ << ", small_factor_cache.size() = " << small_factor_cache.size() << std::endl;
	
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
	
	if (true) {
		setprimes[0] = false;
		setprimes[2/2] = true;
		setprimes[3/2] = true;
		setprimes[4/2] = false;
		aryprimes.push_back(2);
		//divisors_cache = ArrayArray<T, DIVISORS_CACHE_KEYS_LEN, DIVISORS_CACHE_VALUES_LEN>(20066, true);
		//divisors_cache.push_back<1>({0});
	}
	
	if (Globals::verbose) {
		std::cout << "Divisors() line " << __LINE__ << ", small_factor_cache.size() = " << small_factor_cache.size() << std::endl;
		std::cout << "Divisors() setprimes.size() = " << setprimes.size() << std::endl;
		std::cout << "Divisors() aryprimes.size() = " << aryprimes.size() << std::endl;
		std::cout << "Divisors() dynprimes.size() = " << dynprimes.size() << std::endl;
		std::cout << "Divisors() DIVISORS_CACHE_KEYS_LEN = " << DIVISORS_CACHE_KEYS_LEN << std::endl;
		std::cout << "Divisors() DIVISORS_CACHE_VALUES_LEN = " << DIVISORS_CACHE_VALUES_LEN << std::endl;
		std::cout << "Divisors() divisors_cache.keys_size() = " << divisors_cache.keys_size() << std::endl;
		std::cout << "Divisors() divisors_cache.values_size() = " << divisors_cache.values_size() << std::endl;
		std::cout << "Divisors() about to call init_primes() a = 0, b = " << (LEN_SET_PRIMES + LEN_DYN_PRIMES) << ", sqrt_b = " << isqrt3a(LEN_SET_PRIMES + LEN_DYN_PRIMES) << std::endl;
	}

	init_primes(0, LEN_SET_PRIMES + LEN_DYN_PRIMES);
	
	bln_init = true;
}

template<ValidIntegerType T>
void Divisors<T>::init_primes(uint64_t a, uint64_t b) {
	if (Globals::verbose) {
		std::cout << "init_primes() a = " << a << ", b = " << b << std::endl;
		std::cout << "init_primes() setprimes.size() = " << setprimes.size() << std::endl;
		std::cout << "init_primes() aryprimes.size() = " << aryprimes.size() << std::endl;
		std::cout << "init_primes() dynprimes.size() = " << dynprimes.size() << std::endl;
	}
	
	/*
	if (b/2 <= setprimes.size()) {
		return;
	}
	*/
	
	if (a < 5) {
		setprimes[0] = false;
		setprimes[2/2] = true;
		setprimes[3/2] = true;
		setprimes[4/2] = false;
	}
	
    primesieve::iterator it;
    uint64_t p = it.next_prime();
	int64_t imaxp = 1;
	if (aryprimes.size() > 0) {
		try {
			imaxp = aryprimes.back();
		} catch (const std::out_of_range& oor) {
			std::cout << "Range exception: " << oor.what() << std::endl;
			imaxp = 1;
		} catch (const std::exception& ex) {
			std::cout << "Standard exception: " << ex.what() << std::endl;
			imaxp = 1;
		}
	}
	if (Globals::verbose) {
		std::cout << "init_primes() imaxp = " << imaxp << std::endl;
		std::cout << "init_primes() b - 2*setprimes.size() = " << (b - 2*setprimes.size()) << std::endl;
		std::cout << "init_primes() b - imaxp = " << (b - imaxp) << std::endl;
	}
	
    std::vector<std::string> spinner = {"|", "/", "-", "\\"};    
    uint64_t i = 1;
    uint64_t j = 0;
    size_t ilen = spinner.size();
    double prevpct = -0.1;
    double thispct = 0.0;
	
	auto spin = [&](uint64_t inum, uint64_t iden) {
        if (b <= 16777216) {
            return;
        }
        i++;
        if (i % 1000 == 0) {
            thispct = std::round(1000.0 * inum / iden) / 10.0;
            if (thispct >= 99.9) {
                thispct = 100.0;
            }
            if (thispct > prevpct) {
                j++;
                std::cout << "\r" << spinner[j % ilen] << " divisors init_primes(" << format_with_commas(b) << ") at " << std::fixed << std::setprecision(1) << thispct << "%";
                std::cout.flush();
                prevpct = thispct;
            }
        }
    };
	
	T sqrt_b = isqrt3a(b);
	//aryprimes.resize(sqrt_b);
    for (; p < b; p = it.next_prime()) {
		if (Globals::verbose) spin(p - imaxp, b - imaxp);
		if (p <= sqrt_b && p > imaxp) {
			aryprimes.push_back(p);
		}
		if (p/2 < setprimes.size()) {
			setprimes[p/2] = true;
		} else {
			dynprimes[p/2 - setprimes.size()] = true;
		}
    }
	if (Globals::verbose) std::cout << std::endl;

	//sqrt_b = 8192 + 212;
	try {
		divisors_cache = ArrayArray<T, DIVISORS_CACHE_KEYS_LEN, DIVISORS_CACHE_VALUES_LEN>(sqrt_b, true);
		//divisors_cache.push_back<std::vector<T>>({0});
		//divisors_cache.push_back<std::vector<T>>({1});
		divisors_cache.push_back<boost::container::small_vector<T, 1>>({0});
		divisors_cache.push_back<boost::container::small_vector<T, 1>>({1});		
	} catch (const std::bad_alloc& e) {
		std::cout << "Caught std::bad_alloc during divisors_cache init\n";
		std::cout << "Error details: " << e.what() << std::endl;
		return;
	} catch (const std::exception& e) {
		std::cout << "Caught std::exception: " << e.what() << std::endl;
		return;
	} catch (...) {
		std::cout << "Caught an unknown exception.\n";
	}
	for (int i = 2; i <= sqrt_b; i++) {
		if (Globals::verbose) spin(i, sqrt_b);
		//divisors_cache.push_back<DIVISORS_VEC_LEN>(divisors(i));
		//divisors_cache.push_back_small_vector<DIVISORS_VEC_LEN>(divisors(i));
		divisors_cache.push_back(divisors(i));
	}
	if (Globals::verbose) std::cout << std::endl;
	
	if (Globals::verbose) {
		std::cout << "init_primes() setprimes.size() = " << setprimes.size() << std::endl;
		std::cout << "init_primes() aryprimes.size() = " << aryprimes.size() << std::endl;
		std::cout << "init_primes() divisors_cache.size() = " << divisors_cache.size() << ", divisors_cache.values_size() = " << divisors_cache.values_size() << std::endl;
	}
	
	//size_t bytes = get_current_rss();
    //std::cout << "init_primes() memory = " << std::fixed << std::setprecision(2) << bytes / (1024.0 * 1024.0) << " MB" << std::endl;
	
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

}

template<ValidIntegerType T>
bool Divisors<T>::resize(int64_t n) {
	if (Globals::verbose) std::cout << "resize(" << n << "), setprimes.size() = " << setprimes.size() << ", dynprimes.size() = " << dynprimes.size() << std::endl;
	if (n/2 > setprimes.size() + dynprimes.size()) {
		uint64_t ioldset = static_cast<uint64_t>(setprimes.size() - 1);
		uint64_t iolddyn = static_cast<uint64_t>(dynprimes.size() - 1);
		double exp = std::ceil(std::log2(static_cast<double>(n - setprimes.size())));
		if (exp < 26) {
			exp++;
		}
		uint64_t inewdyn = static_cast<uint64_t>(std::pow(2.0, exp));
		try {
			//std::cout << "resize(" << n << ") resizing from " << iolddyn << " to " << (inewdyn/2 + 1) << std::endl;
			dynprimes.resize(inewdyn/2 + 1);
		} catch (const std::bad_alloc& e) {
			std::cout << "Caught std::bad_alloc during dynprimes.resize()\n";
			std::cout << "Error details: " << e.what() << std::endl;
		} catch (const std::exception& e) {
			std::cout << "Caught std::exception: " << e.what() << std::endl;
		} catch (...) {
			std::cout << "Caught an unknown exception.\n";
		}
		init_primes(ioldset + iolddyn, ioldset + inewdyn);
		return true;
	}
	return false;
}

template<ValidIntegerType T>
Divisors<T>& Divisors<T>::get_instance(int64_t n, bool bln_thread_local) {
	PyGILState_STATE gilstate;
	gilstate = PyGILState_Ensure();
	if (Globals::verbose) {
		std::cout << "Divisors<T>::get_instance(), n = " << n << ", bln_init = " << bln_init << std::endl;
		std::cout << "Divisors<T>::get_instance(), DIVISORS_CACHE_KEYS_LEN = " << DIVISORS_CACHE_KEYS_LEN << std::endl;
		std::cout << "Divisors<T>::get_instance(), DIVISORS_CACHE_VALUES_LEN = " << DIVISORS_CACHE_VALUES_LEN << std::endl;
	}
	bool bln = bln_init;
	static Divisors<T>* global_instance = nullptr;
    static thread_local Divisors<T>* local_instance = nullptr;
    Divisors<T>*& instance = bln_thread_local ? local_instance : global_instance;
    if (instance == nullptr) {
        if (!bln_thread_local) {
            static std::mutex get_instance_mtx;
            std::lock_guard<std::mutex> lock(get_instance_mtx);
            if (instance == nullptr) {
                instance = new Divisors<T>();
            }
        } else {
            instance = new Divisors<T>();
        }
    }
	if (Globals::verbose) {
		auto [i, j] = instance->thread_id();
		if (i == 0) {
			i = j;
		}
		std::cout << "Divisors<T>::get_instance(), thread_id = " << i << std::endl;
	}
	instance->resize(n);
	PyGILState_Release(gilstate);
    return *instance;
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
uint64_t Divisors<T>::size() {
	return 2 * setprimes.size() - 1 + 2 * dynprimes.size() - 1;
}

template<ValidIntegerType T>
std::pair<unsigned long, unsigned long long> Divisors<T>::thread_id() {
	std::string id = std::format("{}", std::this_thread::get_id());
	return std::make_pair(PyThread_get_thread_ident(), to_long_long(id));
}

/*
template<ValidIntegerType T>
std::pair<unsigned long, unsigned long long> Divisors<T>::thread_id() {
	return thread_id();
}
*/

/*
import divisors as div
assert(not div.is_prime(0))
assert(not div.is_prime(1))
assert(div.is_prime(2))
assert(div.is_prime(3))
assert(not div.is_prime(4))
assert(div.is_prime(5))
*/
template<ValidIntegerType T>
bool Divisors<T>::is_prime(T n) {
	if (Globals::verbose) std::cout << "is_prime(" << n << "), setprimes.size() = " << setprimes.size() << std::endl;
	if (n/2 < setprimes.size()) {
		return (n == 2) || ((n % 2 == 1) && setprimes[n/2]);
	} else {
		return (n == 2) || ((n % 2 == 1) && dynprimes[n/2 - setprimes.size()]);
	}
	
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
template<size_t N>
T Divisors<T>::mult(const std::array<T, N>& ary) {
    if (ary.size() <= 2) {
        return (is_prime(ary[0]) || is_prime(ary[1])) ? ary[0] * ary[1] : lcm(ary[0], ary[1]);
    } else {
		T i = (is_prime(ary[0]) || is_prime(ary[1])) ? ary[0] * ary[1] : lcm(ary[0], ary[1]);
		for (int a = 2; a < ary.size(); a++) {
			i = (is_prime(ary[a])) ? i * ary[a] : lcm(i, ary[a]);
		}
		return i;
	}
	/*
    if (ary.size() <= 2) {
        return lcm(ary[0], ary[1]);
    } else {
		T i = lcm(ary[0], ary[1]);
		for (int a = 2; a < ary.size(); a++) {
			i = lcm(i, ary[a]);
		}
		return i;
	}
	*/
}

template<ValidIntegerType T>
BoostRational<T> Divisors<T>::calc_density_bitmask(T i, const vector<T>& a, const BoostRational<T>& max_sum) {
	bool bcheckmax = false;
    if (a.size() == 0) {
        return BoostRational<T>(0, 1);
	}
    BoostRational<T> frac(1, a[0]);
    if (frac > max_sum) {
        return BoostRational<T>(1, 1);
	}
    for (uint64_t mask = 1; mask < (1ULL << a.size()); ++mask) {
        if (mask == 1) {
            continue;
		}
        T denom = 1;
        for (int i = 0; i < a.size(); ++i) {
            if (mask & (1ULL << i)) {
				if (denom == 1) {
					denom = a[i];
				} else {
					denom = (is_prime(a[i])) ? denom * a[i] : lcm(denom, a[i]);
				}
            }
        }
        BoostRational<T> term(1, denom);
        if (std::popcount(mask) & 1) {
            frac += term;
        } else {
            frac -= term;
		}
    }
    if (bcheckmax && frac > max_sum) {
        return BoostRational<T>(1, 1);
	}
    return frac;
}

/*

bspan = False
for i in range(2, 9):
	terms = []
	terms.append(f"BoostRational<T>(1, a[{i}])")
	for j in range(1, i + 1):
		for c in itertools.combinations(range(i), j):
			args = ", ".join(f"a[{k}]" for k in c + (i,))
			if bspan:
				terms.append(f" {'-' if j % 2 == 1 else '+'} BoostRational<T>(1, mult({{{args}}}))")
			else:
				terms.append(f" {'-' if j % 2 == 1 else '+'} BoostRational<T>(1, mult(std::array<T, {j+1}>{{{args}}}))")
			line = "\tfrac += " + "".join(terms) + ";"
			line = line.replace("frac += + ", "frac += ")
			line = line.replace("frac += - ", "frac += -")
			if len(line) > 3840:
				print(line)
				terms = []
	if len(terms) > 0:
		line = "\tfrac += " + "".join(terms) + ";"
		line = line.replace("frac += + ", "frac += ")
		line = line.replace("frac += - ", "frac += -")
		print()
	print(f"if (a.size() >= {i+1}) {{")
	print("\treturn frac;")
	print("}")
*/
#undef BLN_CHECKMAX

template<ValidIntegerType T>
BoostRational<T> Divisors<T>::calc_density_unrolled(T i, const std::vector<T>& a, const BoostRational<T>& max_sum) {
    bool bcheckmax = false;
    if (a.size() == 0) {
        return BoostRational<T>(0, 1);
	}
	if (a.size() > 9) {
		return calc_density_bitmask(i, a, max_sum);
	}
    BoostRational<T> frac(1, a[0]);
#ifdef BLN_CHECKMAX
    if (frac > max_sum) {
        return BoostRational<T>(1, 1);
	}
#endif
    frac += BoostRational<T>(1, a[1]) - BoostRational<T>(1, mult(std::array<T, 2>{a[0], a[1]}));
    if (bcheckmax and frac > max_sum) {
        return BoostRational<T>(1, 1);
	}
	if (a.size() < 3) {
		return frac;
	}
	
	//frac += BoostRational<T>(1, a[2]) - BoostRational<T>(1, mult({a[0], a[2]})) - BoostRational<T>(1, mult({a[1], a[2]})) + BoostRational<T>(1, mult({a[0], a[1], a[2]}));
	frac += BoostRational<T>(1, a[2]) - BoostRational<T>(1, mult(std::array<T, 2>{a[0], a[2]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[1], a[2]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[1], a[2]}));
	if (bcheckmax and frac > max_sum) {
		return BoostRational<T>(1, 1);
	}
	if (a.size() < 4) {
		return frac;
	}

	//frac += BoostRational<T>(1, a[3]) - BoostRational<T>(1, mult({a[0], a[3]})) - BoostRational<T>(1, mult({a[1], a[3]})) - BoostRational<T>(1, mult({a[2], a[3]})) + BoostRational<T>(1, mult({a[0], a[1], a[3]})) + BoostRational<T>(1, mult({a[0], a[2], a[3]})) + BoostRational<T>(1, mult({a[1], a[2], a[3]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3]}));
	frac += BoostRational<T>(1, a[3]) - BoostRational<T>(1, mult(std::array<T, 2>{a[0], a[3]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[1], a[3]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[2], a[3]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[1], a[3]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[2], a[3]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[2], a[3]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[2], a[3]}));
	if (bcheckmax and frac > max_sum) {
		return BoostRational<T>(1, 1);
	}
	if (a.size() < 5) {
		return frac;
	}

	//frac += BoostRational<T>(1, a[4]) - BoostRational<T>(1, mult({a[0], a[4]})) - BoostRational<T>(1, mult({a[1], a[4]})) - BoostRational<T>(1, mult({a[2], a[4]})) - BoostRational<T>(1, mult({a[3], a[4]})) + BoostRational<T>(1, mult({a[0], a[1], a[4]})) + BoostRational<T>(1, mult({a[0], a[2], a[4]})) + BoostRational<T>(1, mult({a[0], a[3], a[4]})) + BoostRational<T>(1, mult({a[1], a[2], a[4]})) + BoostRational<T>(1, mult({a[1], a[3], a[4]})) + BoostRational<T>(1, mult({a[2], a[3], a[4]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[4]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[4]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[4]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[4]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4]}));
	frac += BoostRational<T>(1, a[4]) - BoostRational<T>(1, mult(std::array<T, 2>{a[0], a[4]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[1], a[4]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[2], a[4]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[3], a[4]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[1], a[4]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[2], a[4]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[3], a[4]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[2], a[4]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[3], a[4]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[3], a[4]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[2], a[4]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[3], a[4]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[3], a[4]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[3], a[4]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[3], a[4]}));
	if (bcheckmax and frac > max_sum) {
		return BoostRational<T>(1, 1);
	}
	if (a.size() < 6) {
		return frac;
	}

	//frac += BoostRational<T>(1, a[5]) - BoostRational<T>(1, mult({a[0], a[5]})) - BoostRational<T>(1, mult({a[1], a[5]})) - BoostRational<T>(1, mult({a[2], a[5]})) - BoostRational<T>(1, mult({a[3], a[5]})) - BoostRational<T>(1, mult({a[4], a[5]})) + BoostRational<T>(1, mult({a[0], a[1], a[5]})) + BoostRational<T>(1, mult({a[0], a[2], a[5]})) + BoostRational<T>(1, mult({a[0], a[3], a[5]})) + BoostRational<T>(1, mult({a[0], a[4], a[5]})) + BoostRational<T>(1, mult({a[1], a[2], a[5]})) + BoostRational<T>(1, mult({a[1], a[3], a[5]})) + BoostRational<T>(1, mult({a[1], a[4], a[5]})) + BoostRational<T>(1, mult({a[2], a[3], a[5]})) + BoostRational<T>(1, mult({a[2], a[4], a[5]})) + BoostRational<T>(1, mult({a[3], a[4], a[5]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[5]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[5]})) - BoostRational<T>(1, mult({a[0], a[1], a[4], a[5]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[5]})) - BoostRational<T>(1, mult({a[0], a[2], a[4], a[5]})) - BoostRational<T>(1, mult({a[0], a[3], a[4], a[5]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[5]})) - BoostRational<T>(1, mult({a[1], a[2], a[4], a[5]})) - BoostRational<T>(1, mult({a[1], a[3], a[4], a[5]})) - BoostRational<T>(1, mult({a[2], a[3], a[4], a[5]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[5]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[5]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[5]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[5]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[5]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[5]}));
	frac += BoostRational<T>(1, a[5]) - BoostRational<T>(1, mult(std::array<T, 2>{a[0], a[5]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[1], a[5]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[2], a[5]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[3], a[5]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[4], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[1], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[2], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[3], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[4], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[2], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[3], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[4], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[3], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[4], a[5]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[4], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[2], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[3], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[4], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[3], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[4], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[4], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[3], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[4], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[4], a[5]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[4], a[5]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[3], a[5]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[4], a[5]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[4], a[5]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[4], a[5]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[4], a[5]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[4], a[5]}));
	if (bcheckmax and frac > max_sum) {
		return BoostRational<T>(1, 1);
	}
	if (a.size() < 7) {
		return frac;
	}

	//frac += BoostRational<T>(1, a[6]) - BoostRational<T>(1, mult({a[0], a[6]})) - BoostRational<T>(1, mult({a[1], a[6]})) - BoostRational<T>(1, mult({a[2], a[6]})) - BoostRational<T>(1, mult({a[3], a[6]})) - BoostRational<T>(1, mult({a[4], a[6]})) - BoostRational<T>(1, mult({a[5], a[6]})) + BoostRational<T>(1, mult({a[0], a[1], a[6]})) + BoostRational<T>(1, mult({a[0], a[2], a[6]})) + BoostRational<T>(1, mult({a[0], a[3], a[6]})) + BoostRational<T>(1, mult({a[0], a[4], a[6]})) + BoostRational<T>(1, mult({a[0], a[5], a[6]})) + BoostRational<T>(1, mult({a[1], a[2], a[6]})) + BoostRational<T>(1, mult({a[1], a[3], a[6]})) + BoostRational<T>(1, mult({a[1], a[4], a[6]})) + BoostRational<T>(1, mult({a[1], a[5], a[6]})) + BoostRational<T>(1, mult({a[2], a[3], a[6]})) + BoostRational<T>(1, mult({a[2], a[4], a[6]})) + BoostRational<T>(1, mult({a[2], a[5], a[6]})) + BoostRational<T>(1, mult({a[3], a[4], a[6]})) + BoostRational<T>(1, mult({a[3], a[5], a[6]})) + BoostRational<T>(1, mult({a[4], a[5], a[6]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[6]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[6]})) - BoostRational<T>(1, mult({a[0], a[1], a[4], a[6]})) - BoostRational<T>(1, mult({a[0], a[1], a[5], a[6]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[6]})) - BoostRational<T>(1, mult({a[0], a[2], a[4], a[6]})) - BoostRational<T>(1, mult({a[0], a[2], a[5], a[6]})) - BoostRational<T>(1, mult({a[0], a[3], a[4], a[6]})) - BoostRational<T>(1, mult({a[0], a[3], a[5], a[6]})) - BoostRational<T>(1, mult({a[0], a[4], a[5], a[6]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[6]})) - BoostRational<T>(1, mult({a[1], a[2], a[4], a[6]})) - BoostRational<T>(1, mult({a[1], a[2], a[5], a[6]})) - BoostRational<T>(1, mult({a[1], a[3], a[4], a[6]})) - BoostRational<T>(1, mult({a[1], a[3], a[5], a[6]})) - BoostRational<T>(1, mult({a[1], a[4], a[5], a[6]})) - BoostRational<T>(1, mult({a[2], a[3], a[4], a[6]})) - BoostRational<T>(1, mult({a[2], a[3], a[5], a[6]})) - BoostRational<T>(1, mult({a[2], a[4], a[5], a[6]})) - BoostRational<T>(1, mult({a[3], a[4], a[5], a[6]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[6]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[6]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[5], a[6]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[6]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[5], a[6]})) + BoostRational<T>(1, mult({a[0], a[1], a[4], a[5], a[6]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[6]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[5], a[6]})) + BoostRational<T>(1, mult({a[0], a[2], a[4], a[5], a[6]})) + BoostRational<T>(1, mult({a[0], a[3], a[4], a[5], a[6]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[6]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[5], a[6]})) + BoostRational<T>(1, mult({a[1], a[2], a[4], a[5], a[6]})) + BoostRational<T>(1, mult({a[1], a[3], a[4], a[5], a[6]})) + BoostRational<T>(1, mult({a[2], a[3], a[4], a[5], a[6]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[6]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[5], a[6]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[5], a[6]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[5], a[6]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[5], a[6]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[5], a[6]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[5], a[6]}));
	frac += BoostRational<T>(1, a[6]) - BoostRational<T>(1, mult(std::array<T, 2>{a[0], a[6]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[1], a[6]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[2], a[6]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[3], a[6]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[4], a[6]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[1], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[2], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[3], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[4], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[2], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[3], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[4], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[3], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[4], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[4], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[4], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[2], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[3], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[4], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[3], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[4], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[4], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[4], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[3], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[4], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[4], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[4], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[4], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[4], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[4], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[3], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[4], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[4], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[4], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[4], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[4], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[4], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[4], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[4], a[5], a[6]}));
	frac += BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[4], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[4], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[4], a[6]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[4], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[4], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[4], a[5], a[6]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[4], a[5], a[6]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[4], a[5], a[6]}));
	if (bcheckmax and frac > max_sum) {
		return BoostRational<T>(1, 1);
	}
	if (a.size() < 8) {
		return frac;
	}

	//frac += BoostRational<T>(1, a[7]) - BoostRational<T>(1, mult({a[0], a[7]})) - BoostRational<T>(1, mult({a[1], a[7]})) - BoostRational<T>(1, mult({a[2], a[7]})) - BoostRational<T>(1, mult({a[3], a[7]})) - BoostRational<T>(1, mult({a[4], a[7]})) - BoostRational<T>(1, mult({a[5], a[7]})) - BoostRational<T>(1, mult({a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[7]})) + BoostRational<T>(1, mult({a[0], a[2], a[7]})) + BoostRational<T>(1, mult({a[0], a[3], a[7]})) + BoostRational<T>(1, mult({a[0], a[4], a[7]})) + BoostRational<T>(1, mult({a[0], a[5], a[7]})) + BoostRational<T>(1, mult({a[0], a[6], a[7]})) + BoostRational<T>(1, mult({a[1], a[2], a[7]})) + BoostRational<T>(1, mult({a[1], a[3], a[7]})) + BoostRational<T>(1, mult({a[1], a[4], a[7]})) + BoostRational<T>(1, mult({a[1], a[5], a[7]})) + BoostRational<T>(1, mult({a[1], a[6], a[7]})) + BoostRational<T>(1, mult({a[2], a[3], a[7]})) + BoostRational<T>(1, mult({a[2], a[4], a[7]})) + BoostRational<T>(1, mult({a[2], a[5], a[7]})) + BoostRational<T>(1, mult({a[2], a[6], a[7]})) + BoostRational<T>(1, mult({a[3], a[4], a[7]})) + BoostRational<T>(1, mult({a[3], a[5], a[7]})) + BoostRational<T>(1, mult({a[3], a[6], a[7]})) + BoostRational<T>(1, mult({a[4], a[5], a[7]})) + BoostRational<T>(1, mult({a[4], a[6], a[7]})) + BoostRational<T>(1, mult({a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[4], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[5], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[7]})) - BoostRational<T>(1, mult({a[0], a[2], a[4], a[7]})) - BoostRational<T>(1, mult({a[0], a[2], a[5], a[7]})) - BoostRational<T>(1, mult({a[0], a[2], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[3], a[4], a[7]})) - BoostRational<T>(1, mult({a[0], a[3], a[5], a[7]})) - BoostRational<T>(1, mult({a[0], a[3], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[4], a[5], a[7]})) - BoostRational<T>(1, mult({a[0], a[4], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[7]})) - BoostRational<T>(1, mult({a[1], a[2], a[4], a[7]})) - BoostRational<T>(1, mult({a[1], a[2], a[5], a[7]})) - BoostRational<T>(1, mult({a[1], a[2], a[6], a[7]})) - BoostRational<T>(1, mult({a[1], a[3], a[4], a[7]})) - BoostRational<T>(1, mult({a[1], a[3], a[5], a[7]})) - BoostRational<T>(1, mult({a[1], a[3], a[6], a[7]})) - BoostRational<T>(1, mult({a[1], a[4], a[5], a[7]})) - BoostRational<T>(1, mult({a[1], a[4], a[6], a[7]})) - BoostRational<T>(1, mult({a[1], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[2], a[3], a[4], a[7]})) - BoostRational<T>(1, mult({a[2], a[3], a[5], a[7]})) - BoostRational<T>(1, mult({a[2], a[3], a[6], a[7]})) - BoostRational<T>(1, mult({a[2], a[4], a[5], a[7]})) - BoostRational<T>(1, mult({a[2], a[4], a[6], a[7]})) - BoostRational<T>(1, mult({a[2], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[3], a[4], a[5], a[7]})) - BoostRational<T>(1, mult({a[3], a[4], a[6], a[7]})) - BoostRational<T>(1, mult({a[3], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[5], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[5], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[4], a[5], a[7]}));
	//frac += BoostRational<T>(1, mult({a[0], a[1], a[4], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[7]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[5], a[7]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[2], a[4], a[5], a[7]})) + BoostRational<T>(1, mult({a[0], a[2], a[4], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[2], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[3], a[4], a[5], a[7]})) + BoostRational<T>(1, mult({a[0], a[3], a[4], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[3], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[7]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[5], a[7]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[6], a[7]})) + BoostRational<T>(1, mult({a[1], a[2], a[4], a[5], a[7]})) + BoostRational<T>(1, mult({a[1], a[2], a[4], a[6], a[7]})) + BoostRational<T>(1, mult({a[1], a[2], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[1], a[3], a[4], a[5], a[7]})) + BoostRational<T>(1, mult({a[1], a[3], a[4], a[6], a[7]})) + BoostRational<T>(1, mult({a[1], a[3], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[1], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[2], a[3], a[4], a[5], a[7]})) + BoostRational<T>(1, mult({a[2], a[3], a[4], a[6], a[7]})) + BoostRational<T>(1, mult({a[2], a[3], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[2], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[3], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[5], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[5], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[5], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[5], a[7]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[2], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[3], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[5], a[7]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[6], a[7]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[1], a[2], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[1], a[3], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[2], a[3], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[5], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]}));
	frac += BoostRational<T>(1, a[7]) - BoostRational<T>(1, mult(std::array<T, 2>{a[0], a[7]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[1], a[7]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[2], a[7]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[3], a[7]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[4], a[7]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[1], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[2], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[3], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[4], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[2], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[3], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[4], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[3], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[4], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[4], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[4], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[4], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[2], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[3], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[4], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[3], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[4], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[4], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[4], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[4], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[3], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[4], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[4], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[4], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[4], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[4], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[6], a[7]}));
	frac += -BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[4], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[4], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[4], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[4], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[3], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[4], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[4], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[4], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[4], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[4], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[4], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[4], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[4], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[4], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[4], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[4], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[4], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[4], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[4], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[4], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[4], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[3], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[4], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[4], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[4], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[4], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[4], a[6], a[7]}));
	frac += -BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[4], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[4], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[3], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[4], a[5], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[4], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[3], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[2], a[3], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[4], a[5], a[7]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[4], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[3], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[2], a[3], a[4], a[5], a[6], a[7]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[1], a[2], a[3], a[4], a[5], a[6], a[7]})) - BoostRational<T>(1, mult(std::array<T, 8>{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]}));
	if (bcheckmax and frac > max_sum) {
		return BoostRational<T>(1, 1);
	}
	if (a.size() < 9) {
		return frac;
	}

	//frac += BoostRational<T>(1, a[8]) - BoostRational<T>(1, mult({a[0], a[8]})) - BoostRational<T>(1, mult({a[1], a[8]})) - BoostRational<T>(1, mult({a[2], a[8]})) - BoostRational<T>(1, mult({a[3], a[8]})) - BoostRational<T>(1, mult({a[4], a[8]})) - BoostRational<T>(1, mult({a[5], a[8]})) - BoostRational<T>(1, mult({a[6], a[8]})) - BoostRational<T>(1, mult({a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[8]})) + BoostRational<T>(1, mult({a[0], a[3], a[8]})) + BoostRational<T>(1, mult({a[0], a[4], a[8]})) + BoostRational<T>(1, mult({a[0], a[5], a[8]})) + BoostRational<T>(1, mult({a[0], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[8]})) + BoostRational<T>(1, mult({a[1], a[3], a[8]})) + BoostRational<T>(1, mult({a[1], a[4], a[8]})) + BoostRational<T>(1, mult({a[1], a[5], a[8]})) + BoostRational<T>(1, mult({a[1], a[6], a[8]})) + BoostRational<T>(1, mult({a[1], a[7], a[8]})) + BoostRational<T>(1, mult({a[2], a[3], a[8]})) + BoostRational<T>(1, mult({a[2], a[4], a[8]})) + BoostRational<T>(1, mult({a[2], a[5], a[8]})) + BoostRational<T>(1, mult({a[2], a[6], a[8]})) + BoostRational<T>(1, mult({a[2], a[7], a[8]})) + BoostRational<T>(1, mult({a[3], a[4], a[8]})) + BoostRational<T>(1, mult({a[3], a[5], a[8]})) + BoostRational<T>(1, mult({a[3], a[6], a[8]})) + BoostRational<T>(1, mult({a[3], a[7], a[8]})) + BoostRational<T>(1, mult({a[4], a[5], a[8]})) + BoostRational<T>(1, mult({a[4], a[6], a[8]})) + BoostRational<T>(1, mult({a[4], a[7], a[8]})) + BoostRational<T>(1, mult({a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[4], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[5], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[4], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[5], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[3], a[4], a[8]})) - BoostRational<T>(1, mult({a[0], a[3], a[5], a[8]})) - BoostRational<T>(1, mult({a[0], a[3], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[3], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[4], a[5], a[8]})) - BoostRational<T>(1, mult({a[0], a[4], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[4], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[4], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[5], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[6], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[3], a[4], a[8]})) - BoostRational<T>(1, mult({a[1], a[3], a[5], a[8]})) - BoostRational<T>(1, mult({a[1], a[3], a[6], a[8]})) - BoostRational<T>(1, mult({a[1], a[3], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[4], a[5], a[8]})) - BoostRational<T>(1, mult({a[1], a[4], a[6], a[8]})) - BoostRational<T>(1, mult({a[1], a[4], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[1], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[2], a[3], a[4], a[8]})) - BoostRational<T>(1, mult({a[2], a[3], a[5], a[8]})) - BoostRational<T>(1, mult({a[2], a[3], a[6], a[8]})) - BoostRational<T>(1, mult({a[2], a[3], a[7], a[8]})) - BoostRational<T>(1, mult({a[2], a[4], a[5], a[8]})) - BoostRational<T>(1, mult({a[2], a[4], a[6], a[8]})) - BoostRational<T>(1, mult({a[2], a[4], a[7], a[8]})) - BoostRational<T>(1, mult({a[2], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[2], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[2], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[3], a[4], a[5], a[8]})) - BoostRational<T>(1, mult({a[3], a[4], a[6], a[8]})) - BoostRational<T>(1, mult({a[3], a[4], a[7], a[8]})) - BoostRational<T>(1, mult({a[3], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[3], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[3], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[5], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[5], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[4], a[5], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[4], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[4], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[5], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[4], a[5], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[4], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[4], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[3], a[4], a[5], a[8]})) + BoostRational<T>(1, mult({a[0], a[3], a[4], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[3], a[4], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[3], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[3], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[3], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[5], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[6], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[4], a[5], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[4], a[6], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[4], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[3], a[4], a[5], a[8]})) + BoostRational<T>(1, mult({a[1], a[3], a[4], a[6], a[8]})) + BoostRational<T>(1, mult({a[1], a[3], a[4], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[3], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[1], a[3], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[3], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[1], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[2], a[3], a[4], a[5], a[8]})) + BoostRational<T>(1, mult({a[2], a[3], a[4], a[6], a[8]})) + BoostRational<T>(1, mult({a[2], a[3], a[4], a[7], a[8]})) + BoostRational<T>(1, mult({a[2], a[3], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[2], a[3], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[2], a[3], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[2], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[2], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[2], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[2], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[3], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[3], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[3], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[3], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[5], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[6], a[8]}));
	//frac += BoostRational<T>(-1, mult({a[0], a[1], a[2], a[3], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[5], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[5], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[5], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[3], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[3], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[3], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[3], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[5], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[6], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[3], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[1], a[3], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[3], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[3], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[2], a[3], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[2], a[3], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[2], a[3], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[2], a[3], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[2], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[3], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[5], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[3], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[3], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[2], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[3], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[3], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[2], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[1], a[3], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[2], a[3], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[2], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[1], a[3], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[0], a[2], a[3], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult({a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult({a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]}));
	frac += BoostRational<T>(1, a[8]) - BoostRational<T>(1, mult(std::array<T, 2>{a[0], a[8]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[1], a[8]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[2], a[8]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[3], a[8]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[4], a[8]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 2>{a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[1], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[2], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[3], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[4], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[0], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[2], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[3], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[4], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[1], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[3], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[4], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[2], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[4], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[3], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[4], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[4], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[4], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 3>{a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[2], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[3], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[4], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[1], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[3], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[4], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[2], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[4], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[3], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[4], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[4], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[4], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[0], a[6], a[7], a[8]}));
	frac += -BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[3], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[4], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[2], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[4], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[3], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[4], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[4], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[4], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[1], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[4], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[3], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[4], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[4], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[4], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[2], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[4], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[4], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[4], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[3], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 4>{a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[3], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[4], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[2], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[4], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[3], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[4], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[4], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[4], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[1], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[4], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[5], a[8]}));
	frac += BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[3], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[4], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[4], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[4], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[2], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[4], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[4], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[4], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[3], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[0], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[4], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[3], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[4], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[4], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[4], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[2], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[4], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[4], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[4], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[3], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[1], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[4], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[4], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[4], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[3], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[2], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[3], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[3], a[4], a[5], a[7], a[8]}));
	frac += BoostRational<T>(1, mult(std::array<T, 5>{a[3], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[3], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 5>{a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[4], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[3], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[4], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[4], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[4], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[2], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[4], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[4], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[4], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[3], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[1], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[4], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[4], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[4], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[3], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[2], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[3], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[3], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[3], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[3], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[0], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[4], a[5], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[4], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[4], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[3], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[4], a[5], a[7], a[8]}));
	frac += -BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[2], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[3], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[3], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[3], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[3], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[1], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[2], a[3], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[2], a[3], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[2], a[3], a[4], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[2], a[3], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[2], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 6>{a[3], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[4], a[5], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[4], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[4], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[3], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[2], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[3], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[3], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[3], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[3], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[1], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[2], a[3], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[2], a[3], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[2], a[3], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[2], a[3], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[2], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[0], a[3], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[1], a[2], a[3], a[4], a[5], a[6], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[1], a[2], a[3], a[4], a[5], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[1], a[2], a[3], a[4], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[1], a[2], a[3], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[1], a[2], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[1], a[3], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 7>{a[2], a[3], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 8>{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[8]})) - BoostRational<T>(1, mult(std::array<T, 8>{a[0], a[1], a[2], a[3], a[4], a[5], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 8>{a[0], a[1], a[2], a[3], a[4], a[6], a[7], a[8]}));
	frac += -BoostRational<T>(1, mult(std::array<T, 8>{a[0], a[1], a[2], a[3], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 8>{a[0], a[1], a[2], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 8>{a[0], a[1], a[3], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 8>{a[0], a[2], a[3], a[4], a[5], a[6], a[7], a[8]})) - BoostRational<T>(1, mult(std::array<T, 8>{a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]})) + BoostRational<T>(1, mult(std::array<T, 9>{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]}));
	if (bcheckmax and frac > max_sum) {
		return BoostRational<T>(1, 1);
	}
	return frac;
}


template<ValidIntegerType T>
vec_divisors<T> Divisors<T>::divisors(T n) {
    T abs_n = n < 0 ? -n : n;
	resize(static_cast<int64_t>(n));
    if (is_prime(n)) {
		if (Globals::verbose) std::cout << "divisors(" << n << ") returning { 1, " << n << " }" << std::endl;
		vec_divisors<T> result;
        result.push_back(1);
		result.push_back(n);
		return result;
    }
    if (n % 2 == 0 && is_prime(n/2)) {
		if (Globals::verbose) std::cout << "divisors(" << n << ") returning { 1, 2, " << n/2 << ", " << n << " }" << std::endl;
		vec_divisors<T> result;
        result.push_back(1);
		result.push_back(2);
		result.push_back(n/2);
		result.push_back(n);
		return result;
    }
    if (n % 3 == 0 && is_prime(n/3)) {
		if (Globals::verbose) std::cout << "divisors(" << n << ") returning { 1, 3, " << n/3 << ", " << n << " }" << std::endl;
		vec_divisors<T> result;
        result.push_back(1);
		result.push_back(3);
		result.push_back(n/3);
		result.push_back(n);
		return result;
    }
    if (n < divisors_cache.size()) {
		if (Globals::verbose) std::cout << "divisors(" << n << ") returning from divisors_cache " << to_string<T>(divisors_cache.get<vec_divisors<T>>(n)) << std::endl;
        return divisors_cache.get<vec_divisors<T>>(n);
    }
    vec_divisors<T> vec = _divisors(abs_n);
	if (Globals::verbose) std::cout << "divisors(n=" << n << ") returning " << to_string<T>(vec) << std::endl;
    std::sort(vec.begin(), vec.end());
    return vec;
}

//cannot convert argument 2 from 'boost::container::flat_map<T,int, ...>' to 'const std::map<int64_t,int64_t, ...> &
template<ValidIntegerType T>
vec_divisors<T> Divisors<T>::_rec_gen(T n, const map_t_int<T, int>& factors, const vec_divisors<T>& keys) {
    if (n == static_cast<int64_t>(keys.size())) {
		vec_divisors<T> result;
        result.push_back(1);
		return result;
    } else {
        vec_divisors<T> pows;
        pows.push_back(1);
        for (int64_t i = 0; i < factors.at(keys[n]); ++i) {
            pows.push_back(pows.back() * keys[n]);
        }
        vec_divisors<T> next_generation = _rec_gen(n + 1, factors, keys);
        vec_divisors<T> result;
        for (T q : next_generation) {
            for (T p : pows) {
                result.push_back(p * q);
            }
        }
        return result;
    }
}

template<ValidIntegerType T>
vec_divisors<T> Divisors<T>::_divisors(T n) {
    if (n == 0) return { 0 };
    if (n == 1) return { 1 };

    const map_t_int<T, int> factors = factorint(n, 0);
	if (Globals::verbose) std::cout << "factorint(" << n << ") returned " << to_string<T, int>(factors) << std::endl;
	
    vec_divisors<T> ps;
    for (const auto& pair : factors) {
        ps.push_back(pair.first);
    }

    return _rec_gen(0, factors, ps);
}

template<ValidIntegerType T>
std::pair<T, T> Divisors<T>::_factorint_small(map_t_int<T, int>& factors, T n, T limit, int fail_max, T next_p) {
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
bool Divisors<T>::_check_termination(map_t_int<T, int>& factors, T n, T next_p, int call_depth) {
    if (Globals::verbose) std::cout << "_check_termination(factors=" << to_string<T, int>(factors) << ", n = " << n << ", next_p = " << next_p << ", call_depth=" << call_depth << ")" << std::endl;

    if (n == 1 || call_depth >= MAX_RECURSION) {
        return true;
    }

    size_t un = static_cast<size_t>(n);

    if (Globals::verbose) std::cout << "_check_termination() n < next_p * next_p (" << n << " < " << next_p << " * " << next_p << ") ? " << (n < next_p * next_p) << std::endl;
    if (Globals::verbose) std::cout << "_check_termination() n/2 < LEN_SET_PRIMES + dynprimes.size() (" << n/2 << " < " << LEN_SET_PRIMES + dynprimes.size() << ") ? " << (un/2 < LEN_SET_PRIMES + dynprimes.size()) << std::endl;
    if (n < next_p * next_p || (un/2 < LEN_SET_PRIMES + dynprimes.size() && is_prime(n))) {
#ifdef FACTOR_CACHE
        factor_cache[n] = n;
#endif
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
    if (Globals::verbose) std::cout << "_check_termination() base < LEN_SET_PRIMES + dynprimes.size() (" << base << " < " << LEN_SET_PRIMES + dynprimes.size() << ") ? " << (ubase < LEN_SET_PRIMES + dynprimes.size()) << std::endl;
    if (base < next_p * next_p || (ubase/2 < LEN_SET_PRIMES + dynprimes.size() && is_prime(base))) {
#ifdef FACTOR_CACHE
        factor_cache[n] = base;
#endif
        factors[base] = exp;
    } else {
        map_t_int<T, int> facs = factorint(base, call_depth + 1);
        for (const auto& pair : facs) {
            factors[pair.first] = exp * pair.second;
        }
    }
    return true;
}


template<ValidIntegerType T>
std::pair<T, bool> Divisors<T>::_trial(map_t_int<T, int>& factors, T n, const vec_ps& candidates) {
    if (Globals::verbose) std::cout << "_trial(factors=" << to_string<T, int>(factors) << ", n = " << n << ", candidates = " << to_string<T, CANDIDATES_VEC_LEN>(candidates) << ")" << std::endl;

    auto nfactors_before = factors.size();

    for (const T& d : candidates) {
        if (n % d == 0) {
#ifdef FACTOR_CACHE
            if (n != d) {
                factor_cache[n] = d;
            }
#endif
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
std::pair<T, int> Divisors<T>::_perfect_power_done(T current_n, map_t_int<T, int>& current_factors, int current_g, int current_multi) {
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

    map_t_int<T, int> factors;
    int g = 0;
    int multi = 1;

    /*
    std::pair<T, int> done = [&](T current_n, map_t_int<T, int>& current_factors, int current_g, int current_multi) {
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

        vec_divisors<T> factors_values;
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
                } else if (g/2 < LEN_SET_PRIMES + dynprimes.size() && is_prime(g)) {
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
            if (p/2 < LEN_SET_PRIMES + dynprimes.size() && is_prime(p)) {
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
                        } else if (g/2 < LEN_SET_PRIMES + dynprimes.size() && is_prime(g)) {
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
map_t_int<T, int> Divisors<T>::factorint(T n, int call_depth) {
    if (Globals::verbose) std::cout << "factorint(n=" << n << ", call_depth=" << call_depth << ")" << std::endl;

    bool use_trial = true;
    bool use_rho = true;
    bool use_pm1 = true;
    bool use_ecm = false;

    if (n < 0) {
        map_t_int<T, int> factors = factorint(abs(n), call_depth + 1);
        factors[-1] = 1;
        return factors;
    }

    if (n < 100) {
        return small_factor_cache[n];
    }

    map_t_int<T, int> factors;
    if (call_depth >= MAX_RECURSION) {
        return factors;
    }

    T limit = 2 << 14; // 2**15 equivalent.
    int fail_max = 600;

	// _factorint_small(map_t_int<T, int>& factors, T n, T limit, int fail_max, T next_p) 
    auto [remaining_n, next_p] = _factorint_small(factors, n, limit, fail_max, 2);
    n = remaining_n;

    if (next_p == 0) {
        if (n > 1) {
            factors[n] = 1;
        }
        return factors;
    }

#ifdef FACTOR_CACHE
    while (factor_cache.count(n)) {
        T p = factor_cache[n];
        auto [new_n, e] = remove(n, p);
        n = new_n;
        factors[p] = factors.count(p) ? factors[p] + e : e;
    }
#endif

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
                map_t_int<T, int> facs = factorint(r, call_depth + 1);
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
			vec_ps ps;
            size_t idx_low = find(low);
            size_t idx_high = find(high_);
            if (idx_low > -1 && idx_high > -1) {
                //auto end_it = (idx_high > 0 && idx_high <= aryprimes.size()) ? aryprimes.begin() + idx_high : aryprimes.end();
                ps = vec_ps(aryprimes.begin() + idx_low, (idx_high > 0) ? aryprimes.begin() + idx_high : aryprimes.end());
            } else {
                for (int p = low; p < high_; p += 2) {
                    if (is_prime(p)) {
                        ps.push_back(p);
                    }
                }
            }
			if (Globals::verbose) std::cout << "ps.size() = " << ps.size() << ", ps.capacity() = " << ps.capacity() << std::endl;
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
                    vec_ps ps;
                    if (c < next_p * next_p || is_prime(c)) {
                        ps.push_back(c);
                    } else {
                        map_longlong_int factors_map = factorint(c, call_depth + 1);
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
                    vec_ps ps;
                    if (c < next_p * next_p || is_prime(c)) {
                        ps.push_back(c);
                    } else {
                        map_longlong_int factors_map = factorint(c, call_depth + 1);
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
            if (p == 2 || (p > 2 && p % 2 != 0 && p/2 < LEN_SET_PRIMES + dynprimes.size() && is_prime(p))) {
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


Divisors<int64_t>& get_instance(int64_t n, bool bln_thread_local) {
	if (Globals::verbose) {
		std::cout << "get_instance(), n = " << n << std::endl;
	}
	return Divisors<int64_t>::get_instance(n, bln_thread_local);
}

std::vector<int64_t> divisors(int64_t n, bool bln_thread_local = true) {
	/*
	try {
		Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
		return div.divisors(n);
	} catch (const std::bad_alloc& e) {
		std::cout << "Caught std::bad_alloc: " << e.what() << std::endl;
		vec_divisors<int64_t> vec;
		vec.push_back(0);
		return vec;
	} catch (const std::exception& e) {
		std::cout << "Caught std::exception: " << e.what() << std::endl;
		vec_divisors<int64_t> vec;
		vec.push_back(0);
		return vec;
	} catch (...) {
		std::cout << "Caught unknown exception" << std::endl;
		vec_divisors<int64_t> vec;
		vec.push_back(0);
		return vec;
	}
	*/
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
	//return div.divisors(n);
    vec_divisors<int64_t> vec = div.divisors(n);
	return std::vector<int64_t>(vec.begin(), vec.end());
}

uint64_t resize(int64_t n, bool bln_thread_local = true) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
	return div.size();
	//return 2 * staticsetprimes.size() - 1 + 2 * staticdynprimes.size() - 1;	
}

BoostRational<int64_t> calc_density_bitmask(int64_t n, const vector<int64_t>& a, const BoostRational<int64_t>& max_sum, bool bln_thread_local) {
	Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
	return div.calc_density_bitmask(n, a, max_sum);
}

BoostRational<int64_t> calc_density_unrolled(int64_t n, const vector<int64_t>& a, const BoostRational<int64_t>& max_sum, bool bln_thread_local) {
	Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
	return div.calc_density_unrolled(n, a, max_sum);
}

std::pair<size_t, size_t> divisors_cache_size(bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(LEN_SET_PRIMES, bln_thread_local);
	return { div.divisors_cache.keys_size(), div.divisors_cache.values_size() };
}

map_int64t_int factorint(int64_t n, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    return div.factorint(n, 0);    
}    

std::pair<int64_t, int> _factorint_small(int64_t n, int p, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    map_int64t_int factors;
    auto [remaining_n, next_p] = div._factorint_small(factors, n, 2 << 14, 600, p);
    return { remaining_n, next_p };
}

std::pair<int64_t, int> _perfect_power(int64_t n, int p, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    return div._perfect_power(n, p, 0);
}

//std::pair<T, bool> _trial(map_t_int<T, int>& factors, T n, const std::vector<T>& candidates) {
std::pair<int64_t, bool> _trial(map_int64t_int& factors, int64_t n, const vec_ps& candidates, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    return div._trial(factors, n, candidates);
}

//_check_termination(map_t_int<T, int>& factors, T n, T next_p, int call_depth) {
bool _check_termination(map_t_int<int64_t, int>& factors, int64_t n, int64_t next_p, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    return div._check_termination(factors, n, next_p, 1);
}

//pollard_pm1(n, B=low, seed=high_)
//pollard_pm1(T n, int B = 10, T a = 2, int retries = 0, unsigned int seed = 1234) 
int64_t pollard_pm1(int64_t n, int B, unsigned int seed, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    return div.pollard_pm1(n, B, 2, 0, seed);
}

//pollard_rho(n, retries=1, max_steps=low, seed=high_)
//pollard_rho(T n1, T s = 2, T a1 = 1, int retries = 5, unsigned int seed = 1234, uint64_t max_steps = 0)
int64_t pollard_rho(int64_t n, uint64_t max_steps, unsigned int seed, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    return div.pollard_rho(n, 2, 1, 1, seed, max_steps);
}

int64_t isqrt(int64_t n, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    return div.isqrt3a(n);
}

std::pair<int64_t, int64_t> sqrtrem(int64_t n, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    return div.sqrtrem(n);
}

/* 
import divisors as div
import sympy
[n for n in range(134217728 + 1, 134217728 + 65, 2) if div.isprime(n)]
[n for n in range(268435456 + 1, 268435456 + 65, 2) if div.isprime(n)]
[n for n in range(134217728 + 1, 134217728 + 65, 2) if sympy.isprime(n)]
[n for n in range(268435456 + 1, 268435456 + 65, 2) if sympy.isprime(n)]
*/
bool is_prime(int64_t n, bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(n, bln_thread_local);
    return div.is_prime(n);
}

std::vector<uint64_t> c_primes(uint64_t from_limit, uint64_t to_limit = 0) {
    from_limit = std::max(from_limit, uint64_t(0));
    to_limit   = std::max(to_limit,   uint64_t(0));
    if (to_limit == 0) {
        to_limit   = from_limit;
        from_limit = 0;
    }
    errno = 0;
    size_t size = 0;
    void* c_primes = primesieve_generate_primes(from_limit, to_limit, &size, UINT64_PRIMES);
    if (errno != 0) {
        primesieve_free(c_primes);
        throw std::runtime_error("Failed to generate primes, most likely due to insufficient memory.");
    }
    uint64_t* ptr = static_cast<uint64_t*>(c_primes);
    std::vector<uint64_t> vec(ptr, ptr + size);
    primesieve_free(c_primes);
    return vec;
}

std::vector<uint64_t> cpp_primes(uint64_t from_limit, uint64_t to_limit = 0) {
    if (to_limit == 0) {
        to_limit   = from_limit;
        from_limit = 0;
    }
    std::vector<uint64_t> vec;
    try {
        primesieve::generate_primes(from_limit, to_limit, &vec);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to generate primes, most likely due to insufficient memory.");
    }
    return vec;
}

uint64_t len(bool bln_thread_local) {
    Divisors<int64_t>& div = Divisors<int64_t>::get_instance(LEN_SET_PRIMES, bln_thread_local);
	return div.size();
	//return 2 * staticsetprimes.size() - 1 + 2 * staticdynprimes.size() - 1;	
}

void set_verbose(bool v) {
    Globals::verbose = v;
}


/*


Scripts\pip.exe install C:\Users\alex.weslowski\Documents\C++\AlexWeslowski\Divisors
Scripts\pip.exe install D:\C++\AlexWeslowski\Divisors
Scripts\pip.exe install E:\C++\AlexWeslowski\Divisors
python3.14t.exe -m pip install H:\C++\AlexWeslowski\Divisors

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
import threading
import importlib
sys.path.append('C:\\Users\\alex.weslowski\\Documents\\Python\\Sequence')
import sequence
import sympy_ntheory_factor

def threads(id, create_objs):
	print(f"thread_id is {divisors.thread_id()}")
	if create_objs:
		div = divisors.get_instance(1048576)
		comb = divisors.Combinations(1048576)
		print(f"divisors.thread_id is {div.thread_id()}")
		print(f"divisors.size is {div.size()}")
		print(f"combinations.thread_id is {comb.thread_id()}")
	
import divisors
import threading
_ = threading.stack_size(33554432)
divisors.thread_id()
# divisors.set_verbose(True)
div = divisors.get_instance(1048576)
div.thread_id()
bdaemon = False
inumthreads = 4
th = [None] * inumthreads
for t in range(0, inumthreads):
	th[t] = threading.Thread(target=threads, args=(t, True), daemon=bdaemon)
	th[t].start()
for t in range(0, inumthreads):
	th[t].join()

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
div = divisors.Divisors.get_instance(ap)
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
    div = divisors.Divisors.get_instance(max(ary))
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

python3.14t.exe -m pip install D:\C++\AlexWeslowski\Divisors

import subprocess
import sys
import os
import shutil

boost_dir = r"D:\Downloads\boost_1_82_0\boost"
pkg_dir = r"D:\C++\AlexWeslowski\Divisors"
dst_dir = r"D:\C++\AlexWeslowski\Divisors\include\boost"
result = subprocess.run([r"C:\Python\Python314\Scripts\pip3.14.exe", "install", pkg_dir], capture_output=True, text=True)

if result.returncode == 1:
	files = []
	for line in result.stderr.split("\n"):
		print(line)
		if line.find("Cannot open include file") > -1:
			files.append(line)

	for file in files:
		file_path = re.search(r"'boost/.+\.hpp'", file).group().strip("'")
		src_path = os.path.join(boost_dir, file_path).replace("/", "\\").replace("boost\\boost", "boost")
		dst_path = os.path.join(dst_dir, file_path).replace("/", "\\").replace("boost\\boost", "boost")
		match = re.search(r"boost\\.+\\", src_path)
		if match:
			src_path = src_path[:src_path.rfind("\\")+1]
			dst_path = dst_path[:dst_path.rfind("\\")+1]
			_ = shutil.copytree(src_path, dst_path)
		else:
			shutil.copy(path, "backup_folder/")
	
import divisors
import math
import sympy
import time
import random
import sys

t1 = time.time()
for i in range(2, 2**23+1):
	d = divisors.divisors(i)

dt = time.time() - t1
print(f"{i} {round(dt, 2)} {divisors.divisors_cache_size()}")
  8388608  152.44 (40134, 201902)
  8388608    8.45 (40134, 201902) # small_vector, laptop
  8388608    9.96 (40134, 201902) # small_vector, laptop
  8388608   20.62 (40134, 201902) # small_vector, workpc (7.39x)
  8388608   37.08 (40134, 201902) # small_vector, workpc
134217728 2298.13 (40134, 201902)
134217728  513.70 (40134, 201902) # small_vector (4.47x)
# 513.70 sec = 8.56 min

t1 = time.time()
ary_divisors = []
for i in range(2, 2**27+1):
	d = divisors.divisors(i)
	if len(d) > 2:
		ary_divisors.append(len(d))

dt = time.time() - t1
ary_divisors.sort()
ary_divisors[95*len(ary_divisors)//100] #  64
ary_divisors[98*len(ary_divisors)//100] #  96
ary_divisors[99*len(ary_divisors)//100] # 128
ary_divisors[-1]                        # 864

import divisors
divisors.setverbose(True)
divisors.divisors(2 * 3 * 5 * 7)

import divisors
divisors.setverbose(True)
divisors.divisors(2**8 * 3**3 * 5 * 7 * 11 * 23)

i = 2**23 - 5
combinations = divisors.Combinations(i)
combinations.backtrack(i)
list(combinations)

# 6499.76 sec = 108.33 min
# 2802.26 sec =  46.70 min
ary_combo_outer = []
ary_combo_inner = []
ary_keys = []
ary_values = []
t1 = time.time()
for i in range(2, 2**27+1):
	combinations = divisors.Combinations(i)
	combinations.backtrack(i)
	if len(combinations) > 2:
		ary_combo_outer.append(len(combinations))
		ary_combo_inner.append(max([len(c) for c in combinations]))		
		tpl = combinations.aryary_size()
		if tpl[0] > 6:
			ary_keys.append(tpl[0])
		if tpl[1] > 6:
			ary_values.append(tpl[1])

#combinations.get_arrayarray().keys_capacity()   #  724
#combinations.get_arrayarray().values_capacity() # 1948
dt = time.time() - t1
ary_combo_outer.sort()
ary_combo_inner.sort()
ary_keys.sort()
ary_values.sort()
print(f"{i} {round(dt, 2)} {ary_combo_outer[98*len(ary_combo_outer)//100]} {ary_combo_inner[98*len(ary_combo_inner)//100]} {ary_keys[98*len(ary_keys)//100]} {ary_values[98*len(ary_values)//100]} (98 percentile)")
print(f"{i} {round(dt, 2)} {ary_combo_outer[99*len(ary_combo_outer)//100]} {ary_combo_inner[98*len(ary_combo_inner)//100]} {ary_keys[99*len(ary_keys)//100]} {ary_values[99*len(ary_values)//100]} (99 percentile)")
 8388608   698.17      243  688  791 (98 percentile)
 8388608   698.17      362  694 1218 (99 percentile)
33554432 11953.17      354  708 1177 (98 percentile)
33554432 11953.17      594 1188 2028 (99 percentile)
67108864               362  724 1218 (98 percentile)
67108864               681 1362 2329 (99 percentile)
67108864  2386.27  51    5  102  150 (98 percentile)
67108864  2386.27  51    5  102  150 (99 percentile)
134217728 2802.26  51    5  102  150 (98 percentile)
134217728 2802.26  51    5  102  150 (99 percentile)
134217728 6499.76  51    5  102  150 (98 percentile)
134217728 6499.76  51    5  102  150 (99 percentile)

ary_combo_outer.count(3) .. 41226
ary_combo_inner.count(3) .. 
ary_keys.count(6) ......... 41226
ary_values.count(6) ....... 41226
ary_keys.count(8) ....... 1761852
ary_values.count(8) ....... 10953
	

dt = time.time() - t1


divisors.set_verbose(True)
factors = []
combinations = divisors.Combinations()
_ = combinations.backtrack(3293136, 1, [4, 18, 22, 27, 77])
_ = combinations.backtrack(3293136, 1, [6, 8, 9, 21, 363])
_ = combinations.backtrack(3293136, 1, [6, 8, 21, 27, 121])
combinations.to_list()

divisors.set_verbose(False)
combinations = divisors.Combinations()
aryary = combinations.backtrack(3293136, 3293136, [])

sd = sympy.divisors(23*719*3089)
dd = divisors.divisors(23*719*3089)
sd == dd

*/


PYBIND11_MODULE(divisors, m, py::mod_gil_not_used()) {
    m.doc() = "divisors made with pybind11";
	
	/*
	import divisors as div
	import time
	import random
	import statistics
	
	def test(aryary, bitmask=False, unrolled=False):
		one = div.BoostRational(1, 1)
		t0 = time.time()
		if bitmask:
			for ary in aryary:
				frac = div.calc_density_bitmask(1, ary, one)
		elif unrolled:
			for ary in aryary:
				frac = div.calc_density_unrolled(1, ary, one)
		return time.time() - t0
	
	bm = [test(aryary, True, False) for _ in range(10)]
	ur = [test(aryary, False, True) for _ in range(10)]
	print(f"{round(statistics.mean(bm), 2):.2f} +/- {round(statistics.stdev(bm), 2):.2f}")
	print(f"{round(statistics.mean(ur), 2):.2f} +/- {round(statistics.stdev(ur), 2):.2f}")
	
	calc_density_bitmask .... 4.63 +/- 0.35
	calc_density_unrolled ... 4.31 +/- 0.08	
	
	aryary = []
	aryprimes = div.primes(2, 256)
	while len(aryary) < 2**20:
		n = random.randint(2**14, 2**30)
		while n < 2**28 and len(div.divisors(n)) < 24:
			n *= aryprimes[random.randint(0, len(aryprimes)-1)]
		if n < 2**30 and len(div.divisors(n)) >= 24:
			c = div.Combinations(n)
			c.backtrack(n)
			aryary.extend([ary for ary in c if len(ary) >= 4])
	
	*/
	
	m.def("divisors", &divisors);
    m.def("resize", &resize);
	//m.def("get_instance", &get_instance, py::arg("n"), py::arg("bln_thread_local") = true, py::return_value_policy::reference);
	m.def("get_instance", &get_instance, py::return_value_policy::reference);
	m.def("calc_density_bitmask", &calc_density_bitmask);
	m.def("calc_density_unrolled", &calc_density_unrolled);
	m.def("divisors_cache_size", &divisors_cache_size);
    m.def("factorint", &factorint);
    m.def("is_prime", &is_prime);
    m.def("isprime", &is_prime);
    m.def("c_primes", &c_primes);
    m.def("cpp_primes", &cpp_primes);
    m.def("primes", &cpp_primes);
    m.def("primerange", &cpp_primes);
    m.def("prime_range", &cpp_primes);
    m.def("set_verbose", &set_verbose);
    m.def("setverbose", &set_verbose);
    m.def("len", &len);
    m.def("__len__", &len);
    m.def("size", &len);
	m.def("id", &get_id);
	m.def("thread_id", &thread_id);
	
	py::class_<Commons::Math::Rational<int64_t>>(m, "Rational")
		.def(py::init<int64_t, int64_t>(), py::arg("numerator"), py::arg("denominator"))
        .def(py::init<int64_t>())
		.def_property_readonly("numerator", &Commons::Math::Rational<int64_t>::numerator)
		.def_property_readonly("denominator", &Commons::Math::Rational<int64_t>::denominator)
		.def("__repr__", [](const Commons::Math::Rational<int64_t>& r) { return std::to_string(r.numerator()) + "/" + std::to_string(r.denominator()); });
	
	/*
	py::class_<Fraction<int64_t>>(m, "Fraction")
		.def(py::init<int64_t, int64_t>(), py::arg("numerator"), py::arg("denominator"))
        .def(py::init<int64_t>())
		.def_property_readonly("numerator", &Fraction<int64_t>::numerator)
		.def_property_readonly("denominator", &Fraction<int64_t>::denominator)
		.def("__repr__", [](const Fraction<int64_t>& r) { return std::to_string(r.numerator()) + "/" + std::to_string(r.denominator()); });
	*/
	
	py::class_<boost::rational<int64_t>>(m, "BoostRational")
		.def(py::init<int64_t, int64_t>(), py::arg("numerator"), py::arg("denominator"))
        .def(py::init<int64_t>())
		.def_property_readonly("numer", &boost::rational<int64_t>::numer)
		.def_property_readonly("denom", &boost::rational<int64_t>::denom)
		//.def("numerator", &boost::rational<int64_t>::numerator)
		//.def("denominator", &boost::rational<int64_t>::denominator)
		//.def_property("numerator", &boost::rational<int64_t>::numerator)
		//.def_property("denominator", &boost::rational<int64_t>::denominator)
		.def_property("numerator", [](const boost::rational<int64_t>& r) { return r.numerator(); }, [](boost::rational<int64_t>& r, int64_t new_num) { r = boost::rational<int64_t>(new_num, r.denominator()); })
		.def_property("denominator", [](const boost::rational<int64_t>& r) { return r.denominator(); }, [](boost::rational<int64_t>& r, int64_t new_den) { r = boost::rational<int64_t>(r.numerator(), new_den); })
		//.def_property_readonly("numerator", &boost::rational<int64_t>::numerator)
		//.def_property_readonly("denominator", &boost::rational<int64_t>::denominator)
		.def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self <  py::self)
        .def(py::self <= py::self)
        .def(py::self >  py::self)
        .def(py::self >= py::self)
		.def("__hash__", [](const boost::rational<int64_t>& self) { return py::hash(py::make_tuple(self.numerator(), self.denominator())); })
		.def("__repr__", [](const boost::rational<int64_t>& self) { return std::to_string(self.numerator()) + "/" + std::to_string(self.denominator()); });
	
    py::class_<Divisors<int64_t>>(m, "Divisors")
        .def(py::init<>()) 
		.def("get_instance", &Divisors<int64_t>::get_instance, py::return_value_policy::reference)
        //.def("get_instance", &Divisors<int64_t>::get_instance, py::arg("n"), py::arg("bln_thread_local") = true, py::return_value_policy::reference)
		//.def("get_instance", (Divisors<int64_t>& (Divisors<int64_t>::*)(int64_t, bool)) &Divisors<int64_t>::get_instance, py::arg("n"), py::arg("use_thread_local") = true, py::return_value_policy::reference)
		.def("calc_density_bitmask", &Divisors<int64_t>::calc_density_bitmask)
		.def("calc_density_unrolled", &Divisors<int64_t>::calc_density_unrolled)
        .def("divisors", &Divisors<int64_t>::divisors)
		.def("size", &Divisors<int64_t>::size)
		.def("len", &Divisors<int64_t>::size)
		.def("__len__", &Divisors<int64_t>::size)
		.def("id", &Divisors<int64_t>::get_id)
		.def("thread_id", &Divisors<int64_t>::thread_id)
		.def("resize", &Divisors<int64_t>::resize)
        .def("set_verbose", &Divisors<int64_t>::set_verbose);


#include <../src/ArrayArrayPy.cpp>

#include <../src/CombinationsPy.cpp>

//#include <../src/CombinationsIteratorPy.cpp>


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

