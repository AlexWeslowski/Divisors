#include <vector>
#include <memory>
#include <optional>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <../include/ValidInteger.h>
#include <../include/ArrayArray.h>
#include <../include/Combinations.h>


#ifndef COMBINATIONS_ITERATOR_HPP
#define COMBINATIONS_ITERATOR_HPP

template<ValidIntegerType T>
class CombinationsIterator {
public:
    CombinationsIterator();
    CombinationsIterator(const std::shared_ptr<ArrayArray<T, COMBINATIONS_KEYS_LEN, COMBINATIONS_VALUES_LEN>> aryary);

    //std::vector<T> next();
	vec24<T> next();

private:
    const std::shared_ptr<ArrayArray<T, COMBINATIONS_KEYS_LEN, COMBINATIONS_VALUES_LEN>> aryary_;
    size_t index = 0;
};

#endif