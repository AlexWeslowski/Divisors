#include <vector>
#include <memory>
#include <optional>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <../include/ValidInteger.h>
#include <../include/Functions.h>
#include <../include/Divisors.h>
#include <../include/ArrayArray.h>
#include <../include/CombinationsIterator.h>

#ifndef COMBINATIONS_H
#define COMBINATIONS_H


template<ValidIntegerType T>
class Combinations {
private:
    std::optional<bool> verbose;
    std::shared_ptr<ArrayArray<T>> none;
    std::shared_ptr<ArrayArray<T>> aryary;
    T n = 0;
    size_t index = 0;
    static Divisors<T>& get_div();
    

public:
    Combinations(T n);

    size_t min_factors = 2;
    size_t max_factors = 10;
    
    //std::pair<std::vector<T>, std::vector<T>> backtrack(T n, T target, std::vector<T>& factors);
    void backtrack(T target, std::vector<T>& factors);
    void removeAt(size_t idx);
    void remove_at(size_t idx);
    bool get_verbose() const;
    void set_verbose(bool bln);
    size_t size() const;
    size_t len() const;
    std::vector<T> get(size_t idx) const;
    std::vector<T> next();
    std::shared_ptr<ArrayArray<T>> get_arrayarray() const;
    std::vector<std::vector<T>> to_array() const;
    std::vector<std::vector<T>> to_list() const;

};

template<ValidIntegerType T>
Divisors<T>& Combinations<T>::get_div() {
    static Divisors<T> div;
    return div;
}


#endif