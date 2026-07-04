#include "../include/AtomicBitset.h"
#include <../include/AtomicBitReference.h>

template<size_t N>
AtomicBitset<N>::AtomicBitset() {
	for (size_t i = 0; i < NumWords; ++i) {
		words[i].store(0, std::memory_order_relaxed);
	}
}

template<size_t N>
bool AtomicBitset<N>::set(size_t bit_idx) {
	size_t word_idx = bit_idx / BitsPerWord;
	uint64_t mask = 1ULL << (bit_idx % BitsPerWord);
	uint64_t old_word = words[word_idx].fetch_or(mask, std::memory_order_relaxed);
	return (old_word & mask) != 0;
}

template<size_t N>
bool AtomicBitset<N>::reset(size_t bit_idx) {
	size_t word_idx = bit_idx / BitsPerWord;
	uint64_t mask = 1ULL << (bit_idx % BitsPerWord);
	uint64_t old_word = words[word_idx].fetch_and(~mask, std::memory_order_relaxed);
	return (old_word & mask) != 0;
}

template<size_t N>
bool AtomicBitset<N>::test(size_t bit_idx) const {
	size_t word_idx = bit_idx / BitsPerWord;
	uint64_t mask = 1ULL << (bit_idx % BitsPerWord);	
	uint64_t current_word = words[word_idx].load(std::memory_order_relaxed);
	return (current_word & mask) != 0;
}

template<size_t N>
size_t AtomicBitset<N>::size() const {
	return N;
}

template<size_t N>
AtomicBitReference<N> AtomicBitset<N>::operator[](size_t idx) {
	return AtomicBitReference<N>(*this, idx);
}

template<size_t N>
bool AtomicBitset<N>::operator[](size_t idx) const {
	return test(idx);
}


