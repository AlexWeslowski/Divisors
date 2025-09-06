#include <iostream>
#include <cmath>
#include <vector>
#include <cstdint>
#include <concepts>
#include <type_traits>
#include <../include/ValidInteger.h>

#ifndef POINT_H
#define POINT_H

template <ValidIntegerType T>
struct Point {
    T x_cord;
    T z_cord;
    T a_24;
    T mod;

    Point(T x_cord, T z_cord, T a_24, T mod);

    bool operator==(const Point& other) const;

    Point add(const Point& Q, const Point& diff) const;

    Point double_point() const;

    Point mont_ladder(T k) const;

    std::vector<T> to_array() const;
};

#endif // POINT_H