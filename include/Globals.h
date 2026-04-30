#ifndef GLOBALS_H
#define GLOBALS_H

const size_t DIVISORS_VEC_LEN = 24;
const size_t FACTORS_VEC_LEN = 32;
/*
import resource
resource.setrlimit(resource.RLIMIT_STACK, 2**23)
*/
// 65536*8 + 327680*8 = 3145728
// 20480*8 + 102400*8 =  983040
// 16384*8 + 81920*8  =  786432
const size_t DIVISORS_CACHE_KEYS_LEN = 16384;
const size_t DIVISORS_CACHE_VALUES_LEN = 81920;

//const size_t COMBINATIONS_KEYS_LEN = 708;
//const constexpr size_t COMBINATIONS_VALUES_LEN = 1178;
const size_t COMBINATIONS_KEYS_LEN = 724;
const constexpr size_t COMBINATIONS_VALUES_LEN = 1218;


static class Globals {
public:
    static bool verbose;
};


#endif
