// Fraction.cpp by Andrew Li
// used lzz to separate out header from main cpp file

#include "Fraction.h"
#define LZZ_INLINE inline
using namespace std;

template<typename T>
T Fraction<T>::numerator() const {
	return this->_num;
}
template<typename T>
T Fraction<T>::denominator() const {
	return this->_den;
}
template<typename T>
T Fraction<T>::gcd (T first, T second)
                                       {
		if (second == 0) {
			return first;
		}
		return gcd(second, first % second);
	}
template<typename T>
T Fraction<T>::lcm (T first, T second)
                                       {
		return (first * second) / gcd(first, second);
	}
template<typename T>
Fraction Fraction<T>::apply_lcm (T lcm, Fraction<T> const & rhs, Fraction<T> const & lhs)
                                                                              {
		T top1 = rhs._num * (lcm / rhs._den);
		T top2 = lhs._num * (lcm / lhs._den);
		T bottom = rhs._den * (lcm / rhs._den);
		Fraction fraction(top2 + top1, bottom);
		return fraction;
	}
template<typename T>
Fraction Fraction<T>::apply_lcm_minus (T lcm, Fraction<T> const & rhs, Fraction<T> const & lhs)
                                                                              {
		T top1 = rhs._num * (lcm / rhs._den);
		T top2 = lhs._num * (lcm / lhs._den);
		T bottom = rhs._den * (lcm / rhs._den);
		Fraction fraction(top2 - top1, bottom);
		return fraction;
	}
template<typename T>
vector<T> Fraction<T>::comparison_test (T lcm, Fraction<T> const & rhs, Fraction<T> const & lhs)
                                                                                        {
		T top1 = rhs._num * (lcm / rhs._den);
		T top2 = lhs._num * (lcm / lhs._den);
		vector<T> comparsions {top2, top1};
		return comparsions;
	}
template<typename T>
Fraction<T>::Fraction() {
	this->_num = 0;
	this->_den = 1;
}
template<typename T>
Fraction<T>::Fraction(T top, T bottom)
                                              {
		if (bottom == 0) {
			throw "You cannot have 0 as a denominator. Try Again.";
		} else if (bottom < 0) {
			top = -top;
			bottom = -bottom;
		}
		// gets gcd and simplifies if gcd not 1 or 0
		const T gcd = this->gcd(abs(top), abs(bottom));
		if (gcd != 1 && gcd != 0) {
			top /= gcd;
			bottom /= gcd;
		}
		this->_num = top;
		this->_den = bottom;
	}
template<typename T>
Fraction<T>::Fraction(T top)
                              {
		this->_num = top;
		this->_den = 1;
	}
template<typename T>
Fraction Fraction<T>::operator - ()
                             {
		return Fraction(-this->_num, this->_den);
	}
template<typename T>
Fraction Fraction<T>::operator + (Fraction<T> const & rhs)
                                                  {
		T lcm = this->lcm(rhs._den, this->_den);
		return apply_lcm(lcm, rhs, (*this));
	}
template<typename T>
Fraction Fraction<T>::operator - (Fraction<T> const& rhs)
                                            {
		T lcm = this->lcm(rhs._den, this->_den);
		return apply_lcm_minus(lcm, rhs, (*this));
	}
template<typename T>
Fraction Fraction<T>::operator * (Fraction<T> const & rhs)
                                                  {
		Fraction newFraction(this->_num * rhs._num, this->_den * rhs._den);
		return newFraction;
	}
template<typename T>
Fraction Fraction<T>::operator / (Fraction<T> const & rhs)
                                                  {
		Fraction newFraction(this->_num * rhs._den, this->_den * rhs._num);
		return newFraction;
	}
template<typename T>
bool Fraction<T>::operator == (Fraction<T> const & rhs)
                                               {
		T lcm = this->lcm(rhs._den, this->_den);
		vector<T> results = this->comparison_test(lcm, rhs, (*this));
		T left = results[0];
		T right = results[1];
		return (left == right);
	}
template<typename T>
bool Fraction<T>::operator != (Fraction<T> const & rhs)
                                               {
		T lcm = this->lcm(rhs._den, this->_den);
		vector<T> results = this->comparison_test(lcm, rhs, (*this));
		T left = results[0];
		T right = results[1];
		return (left != right);
	}
template<typename T>
bool Fraction<T>::operator < (Fraction<T> const & rhs)
                                              {
		T lcm = this->lcm(rhs._den, this->_den);
		vector<T> results = this->comparison_test(lcm, rhs, (*this));
		T left = results[0];
		T right = results[1];
		return (left < right);
	}
template<typename T>
bool Fraction<T>::operator > (Fraction<T> const & rhs)
                                              {
		T lcm = this->lcm(rhs._den, this->_den);
		vector<T> results = this->comparison_test(lcm, rhs, (*this));
		T left = results[0];
		T right = results[1];
		return (left > right);
	}
template<typename T>
bool Fraction<T>::operator <= (Fraction<T> const & rhs)
                                               {
		T lcm = this->lcm(rhs._den, this->_den);
		vector<T> results = this->comparison_test(lcm, rhs, (*this));
		T left = results[0];
		T right = results[1];
		return (left <= right);
	}
template<typename T>
bool Fraction<T>::operator >= (Fraction<T> const & rhs)
                                               {
		T lcm = this->lcm(rhs._den, this->_den);
		vector<T> results = this->comparison_test(lcm, rhs, (*this));
		T left = results[0];
		T right = results[1];
		return (left >= right);
	}
template<typename T>
Fraction<T>::~ Fraction ()
                    {
		_num = 0;
		_den = 1;
	}
template<typename T>
float Fraction<T>::toDecimal () const
                                {
		return this->_num /(float) this->_den;
	}
template<typename T>
void Fraction<T>::prT ()
                     {
		cout << _num << "/" << _den << endl;
	}

ostream& operator << (ostream& os, Fraction fraction) {
	if (fraction._den == 1) {
		os << fraction._num;
	} else {
		os << fraction._num << "/" << fraction._den;
	}
	return os;
}
#undef LZZ_INLINE
