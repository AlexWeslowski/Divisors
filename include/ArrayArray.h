#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <../include/ValidInteger.h>
#include <../include/Globals.h>
#include <../include/Stringify.h>


#ifndef ARRAY_ARRAY_HPP
#define ARRAY_ARRAY_HPP


template<ValidIntegerType T>
class ArrayArray {
public:
    std::vector<T> keys;
    std::vector<T> values;

    ArrayArray();
    ArrayArray(size_t capacity, bool resizeable);

    void append(const std::vector<T>& ary);
    void push_back(const std::vector<T>& ary);
    void removeAt(size_t idx);
    size_t getSize() const;
    std::vector<T> getItem(size_t idx) const;
    std::vector<std::vector<T>> to_array() const;

private:
    size_t size_;
    size_t keys_capacity_;
    size_t values_capacity_;
    size_t keys_index_;
    size_t values_index_;
    bool resizeable_;
};


#endif // ARRAY_ARRAY_HPP
