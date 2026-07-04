#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <../include/boost/container/small_vector.hpp>
#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <../include/ValidInteger.h>
#include <../include/Functions.h>
#include <../include/Globals.h>
#include <../include/Divisors.h>
#include <../include/ArrayArray.h>
//#include <../include/CombinationsIterator.h>

#ifndef COMBINATIONS_H
#define COMBINATIONS_H


template<ValidIntegerType T>
class Combinations {
private:
    std::optional<bool> verbose;
    //std::shared_ptr<ArrayArray<T, 1, 1>> none;
    std::shared_ptr<ArrayArray<T, COMBINATIONS_KEYS_LEN, COMBINATIONS_VALUES_LEN>> aryary;
	size_t th = 0;
    T n = 0;
    size_t index = 0;
	bool bln_thread_local = true;
	bool gt_half = false;
	bool write_to_file = false;
	std::ofstream file_stream;
    static Divisors<T>& get_div(size_t th, T n, bool bln_thread_local);
    Divisors<T>& get_div();

public:
    Combinations(T n, bool bln_thread_local = true);

    size_t min_factors = 2;
    size_t max_factors = 10;
    
    //std::pair<std::vector<T>, std::vector<T>> backtrack(T n, T target, std::vector<T>& factors);
	void backtrack(T target);
    //void backtrack(T target, std::vector<T>& factors);
	bool is_divisible(vec_factors<T>& factors, size_t factors_len, T fact);
	void _backtrack(size_t idepth, T istart, T target, vec_factors<T>& factors);
    void removeAt(size_t idx);
    void remove_at(size_t idx);
    bool get_verbose() const;
    void set_verbose(bool bln);
    size_t size() const;
    size_t len() const;
	std::pair<unsigned long, unsigned long long> thread_id();
	std::pair<size_t, size_t> aryary_size() const;
    std::pair<size_t, size_t> aryary_len() const;
	std::pair<size_t, size_t> aryary_capacity() const;
    //std::vector<T> get(size_t idx) const;
    //std::vector<T> next();
    vec24<T> get(size_t idx) const;
    vec24<T> next();
    std::shared_ptr<ArrayArray<T, COMBINATIONS_KEYS_LEN, COMBINATIONS_VALUES_LEN>> get_arrayarray() const;
    //std::vector<std::vector<T>> to_array() const;
    //std::vector<std::vector<T>> to_list() const;
	vec384<vec24<T>> to_array() const;
    vec384<vec24<T>> to_list() const;
};

/*
template<ValidIntegerType T>
Divisors<T>& Combinations<T>::get_div() {
    static Divisors<T> div;
    return div;
}
*/
template<ValidIntegerType T>
Divisors<T>& Combinations<T>::get_div(size_t th, T n, bool bln_thread_local) {
	/*
	static boost::container::small_vector<Divisors<T>, 2> instances; 
	while (idx >= instances.size()) {
		Divisors<T> div;
		div.resize(this->n)
		instances.push_back(div);
	}
	return instances[idx];
	*/
	return Divisors<T>::get_instance(n, bln_thread_local);
}
template<ValidIntegerType T>
Divisors<T>& Combinations<T>::get_div() {
	return get_div(this->th, this->n, this->bln_thread_local);
}


#endif