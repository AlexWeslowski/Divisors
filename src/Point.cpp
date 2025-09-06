#include <../include/Point.h>

template <ValidIntegerType T>
Point<T>::Point(T x_cord, T z_cord, T a_24, T mod) : x_cord(x_cord), z_cord(z_cord), a_24(a_24), mod(mod) {
}

template <ValidIntegerType T>
bool Point<T>::operator==(const Point& other) const {
    if (this->a_24 != other.a_24 || this->mod != other.mod) {
        return false;
    }
    return (this->x_cord * other.z_cord) % this->mod == (other.x_cord * this->z_cord) % this->mod;
}

template <ValidIntegerType T>
Point<T> Point<T>::add(const Point& Q, const Point& diff) const {
    T u = (x_cord - z_cord) * (Q.x_cord + Q.z_cord);
    T v = (x_cord + z_cord) * (Q.x_cord - Q.z_cord);
    T add_val = u + v;
    T subt = u - v;
    T new_x_cord = (diff.z_cord * add_val % mod * add_val % mod);
    T new_z_cord = (diff.x_cord * subt % mod * subt % mod);
    return Point<T>(new_x_cord, new_z_cord, a_24, mod);
}

template <ValidIntegerType T>
Point<T> Point<T>::double_point() const {
    T u = (x_cord + z_cord) * (x_cord + z_cord) % mod;
    T v = (x_cord - z_cord) * (x_cord - z_cord) % mod;
    T diff = u - v;
    T new_x_cord = u * v % mod;
    T new_z_cord = diff * (v + a_24 * diff) % mod;
    return Point<T>(new_x_cord, new_z_cord, a_24, mod);
}

template <ValidIntegerType T>
Point<T> Point<T>::mont_ladder(T k) const {
    Point Q = *this;
    Point R = this->double_point();
    T ibits = 0;
    if (k > 0) {
        ibits = floor(log2(k)) + 1;
    }
    for (T i = ibits - 2; i >= 0; --i) {
        if ((k >> i) & 1) {
            Q = Q.add(R, *this);
            R = R.double_point();
        }
        else {
            R = Q.add(R, *this);
            Q = Q.double_point();
        }
    }
    return Q;
}

template <ValidIntegerType T>
std::vector<T> Point<T>::to_array() const {
    return { x_cord, z_cord, a_24, mod };
}
