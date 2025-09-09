#include <vector>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <../include/ValidInteger.h>
#include <../include/Stringify.h>
#include <../include/Divisors.h>
#include <../include/ArrayArray.h>

#ifndef COMBINATIONS_H
#define COMBINATIONS_H


template<ValidIntegerType T>
class Combinations {
private:
    std::shared_ptr<ArrayArray<T>> none;
    std::shared_ptr<ArrayArray<T>> combinations;
    size_t min_factors = 3;
    size_t max_factors = 10;
    static Divisors<T>& get_div();

public:
    Combinations();
    //std::pair<std::vector<T>, std::vector<T>> backtrack(T n, T target, std::vector<T>& factors);
    std::shared_ptr<ArrayArray<T>> backtrack(T n, T target, std::vector<T>& factors);
    void set_verbose(bool bln);

};

template<ValidIntegerType T>
Divisors<T>& Combinations<T>::get_div() {
    static Divisors<T> div;
    return div;
}


#endif