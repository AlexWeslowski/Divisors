#include <vector>
#include <iostream>

#ifndef STRINGIFY_H
#define STRINGIFY_H

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

using namespace std;

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