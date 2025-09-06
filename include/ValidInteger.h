#include <cstdint>
#include <concepts>
#include <type_traits>


#ifndef VALID_INTEGER_H
#define VALID_INTEGER_H

#ifdef __GNUC__
using int128_t = __int128;
using uint128_t = unsigned __int128;
#endif

template <typename T>
concept ValidIntegerType =
std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value || std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value
#ifdef __GNUC__
|| std::is_same<T, int128_t>::value || std::is_same<T, uint128_t>::value
#endif
;

#endif

