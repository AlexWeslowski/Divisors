#pragma once

#include <cstdint>

#ifndef ATOMICBITREFERENCE_H
#define ATOMICBITREFERENCE_H

template <size_t N> 
class AtomicBitset; 

template <size_t N>
class AtomicBitReference {
private:
	AtomicBitset<N>& bitset;
	size_t bit_idx;

public:
	AtomicBitReference(AtomicBitset<N>& b, size_t idx);
	AtomicBitReference<N>& operator=(bool val);
	AtomicBitReference<N>& operator=(const AtomicBitReference<N>& other);
	operator bool() const;
};

#endif