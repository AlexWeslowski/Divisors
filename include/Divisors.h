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
#include <../include/primesieve.hpp>

#ifndef DIVISORS_H
#define DIVISORS_H

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

static bool bln_init = false;
static bool verbose = true;
// 2^24 = 16777216
// 2^25 = 33554432
// 2^26 = 67108864
const int LEN_PRIMES = 16777216;
const int MAX_RECURSION = 10;
static std::vector<std::map<int64_t, int>> small_factor_cache(100);
static std::map<int64_t, int64_t> factor_cache;
static std::bitset<67108864 + 1> setprimes;

#endif