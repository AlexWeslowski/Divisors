#include <concepts>
#include <iostream>
#include <map>
#include <vector>
#include <../include/boost/container/flat_map.hpp>
#include <../include/boost/container/small_vector.hpp>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

using namespace std;

//template<typename T, size_t N>
//using vec_t_n = boost::container::small_vector<T, N>;
//template<typename T, size_t N>
//using vec_t_n = std::vector<T>;

//template<typename T1, typename T2>
//using map_t1_t2 = boost::container::flat_map<T1, T2>;
//template<typename T1, typename T2>
//using map_t1_t2 = std::map<T1, T2>;


/*
template <typename T>
concept VectorLike = requires(T v) {
    { v.push_back(val) };
    { v.size() } -> std::convertible_to<std::size_t>;
};
*/

template <typename C, typename T>
concept VectorLike = requires(C v, T val) {
    v.begin();
    v.capacity();
    v.end();
    v.push_back(val);
    v.size();
    typename C::value_type;
} && std::convertible_to<T, typename C::value_type>;

template<typename T>
std::string to_string(const std::vector<T>& vec) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1)
            oss << ", ";
    }
    oss << "]";
    return oss.str();
}

template<typename T, size_t N>
std::string to_string(const boost::container::small_vector<T, N>& vec) {
	if (vec.empty()) return "[]";
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1)
            oss << ", ";
    }
    oss << "]";
    return oss.str();
}

template<typename T, size_t N>
std::string to_string(const std::vector<T>& vec) {
	if (vec.empty()) return "[]";
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1)
            oss << ", ";
    }
    oss << "]";
    return oss.str();
}

template<size_t N>
std::string to_string(const boost::container::small_vector<int64_t, N>& vec) {
    if (vec.empty()) return "[]";
    std::stringstream ss;
    ss << "[";
    std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<int64_t>(ss, ", "));
    ss << vec.back();
    ss << "]";
    return ss.str();
}

template<size_t N>
std::string to_string(const std::vector<int64_t>& vec) {
    if (vec.empty()) return "[]";
    std::stringstream ss;
    ss << "[";
    std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<int64_t>(ss, ", "));
    ss << vec.back();
    ss << "]";
    return ss.str();
}

template<typename T1, typename T2>
std::string to_string(const boost::container::flat_map<T1, T2>& map) {	
    std::ostringstream oss;
    oss << "{";
    auto it = map.begin();
    while (it != map.end()) {
        oss << it->first << ": " << it->second;
        ++it;
        if (it != map.end()) {
            oss << ", ";
        }
    }
    oss << "}";
    return oss.str();
}

template<typename T1, typename T2>
std::string to_string(const std::map<T1, T2>& map) {
    std::ostringstream oss;
    oss << "{";
    auto it = map.begin();
    while (it != map.end()) {
        oss << it->first << ": " << it->second;
        ++it;
        if (it != map.end()) {
            oss << ", ";
        }
    }
    oss << "}";
    return oss.str();
}


#endif