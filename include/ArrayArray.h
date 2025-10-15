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
#include <../include/Functions.h>

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
    void remove_at(size_t idx);
    size_t size() const;
    size_t len() const;
    size_t values_size() const;
    size_t values_len() const;
    std::vector<T> get(size_t idx) const;
    std::vector<std::vector<T>> to_array() const;
    std::vector<std::vector<T>> to_list() const;

private:
    size_t size_ = 0;
    size_t keys_capacity_ = 0;
    size_t values_capacity_ = 0;
    size_t keys_index_ = 0;
    size_t values_index_ = 0;
    bool resizeable_ = false;
};


#endif // ARRAY_ARRAY_HPP
