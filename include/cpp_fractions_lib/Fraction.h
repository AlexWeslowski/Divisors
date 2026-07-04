// Fraction.h by Andrew Li

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

#ifndef LZZ_Fraction_h
#define LZZ_Fraction_h
#define LZZ_INLINE inline
template<typename T>
class Fraction
{
private:
  T _num;
  T _den;
  T gcd (T, T);
  T lcm (T, T);
  Fraction<T> apply_lcm (T, Fraction<T> const &, Fraction<T> const &);
  Fraction<T> apply_lcm_minus (T, Fraction<T> const &, Fraction<T> const &);
  std::vector<T> comparison_test (T, Fraction<T> const &, Fraction<T> const &);
public:
  Fraction();
  Fraction(T top, T bottom);
  Fraction(T top);
  Fraction<T> operator - ();
  Fraction<T> operator + (Fraction<T> const &);
  Fraction<T> operator - (Fraction<T> const &);
  Fraction<T> operator * (Fraction<T> const &);
  Fraction<T> operator / (Fraction<T> const & );
  bool operator == (Fraction<T> const & );
  bool operator != (Fraction<T> const & );
  bool operator < (Fraction<T> const & );
  bool operator > (Fraction<T> const & );
  bool operator <= (Fraction<T> const & );
  bool operator >= (Fraction<T> const & );
  ~ Fraction ();
  T numerator() const;
  T denominator() const;
  float toDecimal () const;
  void prT ();
  friend std::ostream & operator << (std::ostream & , Fraction<T> ) ;
};
template<typename T>
std::ostream & operator << (std::ostream & , Fraction<T> );
#undef LZZ_INLINE
#endif
