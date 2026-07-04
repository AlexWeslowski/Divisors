#include <concepts>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <../include/boost/container/small_vector.hpp>
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

#define ARRAY_ARRAY_SMALLVEC
#undef SMALLVEC

template<ValidIntegerType T, size_t N>
#ifdef SMALLVEC
	using vec_push = boost::container::small_vector<T, N>;
#else
	using vec_push = std::vector<T>;
#endif

#ifdef SMALLVEC
	template<typename t>
	using vec24 = boost::container::small_vector<t, 24>;
	template<typename t>
	using vec384 = boost::container::small_vector<t, 384>;
#else
	template<typename T>
	using vec24 = std::vector<T>;
	template<typename T>
	using vec384 = std::vector<T>;
#endif

template<ValidIntegerType T, size_t NK, size_t NV>
class ArrayArray {
	static_assert(!std::is_reference_v<T>, "T cannot be a reference");
    static_assert(std::is_object_v<T>, "T must be a complete object type");
	
public:
#ifdef ARRAY_ARRAY_SMALLVEC
    boost::container::small_vector<T, NK> keys;
    boost::container::small_vector<T, NV> values;
	std::vector<T> large_keys;
    std::vector<T> large_values;
#else
    std::vector<T> keys;
    std::vector<T> values;
#endif

    ArrayArray();
    ArrayArray(size_t capacity, bool resizeable);

    //void append(const std::vector<T>& ary);
    //void push_back(const std::vector<T>& ary);
	//template<size_t N>
    //void append(const vec_push<T, N>& ary);
	//void append(const std::vector<T>& ary);
	template<VectorLike<T> V>
    void append(const V& ary);
	template<size_t N>
	void append_small_vector(const boost::container::small_vector<T, N>& ary);
	//template<size_t N>
    //void push_back(const vec_push<T, N>& ary);
	//void push_back(const std::vector<T>& ary);
	template<VectorLike<T> V>
	void push_back(const V& ary);
	template<size_t N>
	void push_back_small_vector(const boost::container::small_vector<T, N>& ary);
    void removeAt(size_t idx);
    void remove_at(size_t idx);
	void clear();
    size_t size() const;
    size_t len() const;
	size_t keys_size() const;
    size_t keys_len() const;
	size_t keys_capacity() const;
    size_t values_size() const;
    size_t values_len() const;
	size_t values_capacity() const;
	//std::string to_string(const std::vector<int64_t>& vec) const;
    //std::vector<T> get(size_t idx) const;
    //std::vector<std::vector<T>> to_array() const;
    //std::vector<std::vector<T>> to_list() const;
	template<VectorLike<T> V>
	V get(size_t idx) const;
    vec384<vec24<T>> to_array() const;
    vec384<vec24<T>> to_list() const;

private:
    size_t size_ = 0;
    size_t keys_capacity_ = 0;
    size_t values_capacity_ = 0;
    size_t keys_index_ = 0;
    size_t values_index_ = 0;
    bool resizeable_ = false;
};


#endif // ARRAY_ARRAY_HPP
