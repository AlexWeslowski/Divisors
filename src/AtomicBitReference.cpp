#include "../include/AtomicBitReference.h"
#include "../include/AtomicBitset.h"

template <size_t N>
AtomicBitReference<N>::AtomicBitReference(AtomicBitset<N>& b, size_t idx) : bitset(b), bit_idx(idx) {}

template <size_t N>
AtomicBitReference<N>& AtomicBitReference<N>::operator=(bool val) {
	if (val) {
		bitset.set(bit_idx);
	} else {
		bitset.reset(bit_idx);
	}
	return *this;
}

template <size_t N>
AtomicBitReference<N>& AtomicBitReference<N>::operator=(const AtomicBitReference<N>& other) {
	return *this = static_cast<bool>(other);
}

template <size_t N>
AtomicBitReference<N>::operator bool() const {
	return bitset.test(bit_idx);
}
