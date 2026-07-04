#pragma once

#include <iostream>
#include <vector>
#include <atomic>

#ifndef ATOMICBITSET_H
#define ATOMICBITSET_H

template <size_t N>
class AtomicBitReference;

template <size_t N>
class AtomicBitset {
private:
    static constexpr size_t BitsPerWord = 64;
    static constexpr size_t NumWords = (N + BitsPerWord - 1) / BitsPerWord;
    std::atomic<uint64_t> words[NumWords]{};

public:
    AtomicBitset();
    bool set(size_t bit_idx);
    bool reset(size_t bit_idx);
    bool test(size_t bit_idx) const;
    size_t size() const;
	AtomicBitReference<N> operator[](size_t idx);
	bool operator[](size_t idx) const;
};

#endif