#pragma once
#include <fraction/Fraction.h>

namespace Quantity {
using Fraction::frac;

template <frac L = 0, frac M = 0,  //
          frac T = 0, frac I = 0>
class Quantity_base {
 public:
  Quantity_base() {}
  Quantity_base(double value) : value(value) {}

  double value;

  void repl() {
    std::cout << "L: " << L << std::endl;
    std::cout << "M: " << M << std::endl;
    std::cout << "T: " << T << std::endl;
    std::cout << "I: " << I << std::endl;
  }

  constexpr frac<> get_L() { return L; }
  constexpr frac<> get_M() { return M; }
  constexpr frac<> get_T() { return T; }
  constexpr frac<> get_I() { return I; }

  template <frac LL, frac MM, frac TT, frac II>
  constexpr bool is_same_dim_as(Quantity_base<LL, MM, TT, II>) {
    return (L == LL) && (M == MM) && (T == TT) && (I == II);
  }

  // Allow same dimenthion add / sub
  constexpr Quantity_base<L, M, T, I> add(const Quantity_base<L, M, T, I> &x) {
    return {value + x.value};
  }

  constexpr Quantity_base<L, M, T, I> sub(const Quantity_base<L, M, T, I> &x) {
    return {value - x.value};
  }

  template <frac L2, frac M2, frac T2, frac I2>
  constexpr Quantity_base<L + L2, M + M2, T + T2, I + I2> mul(
      const Quantity_base<L2, M2, T2, I2> &x) {
    return {value * x.value};
  }

  constexpr Quantity_base<L, M, T, I> mul(const double x) {
    return {value * x};
  }

  template <frac L2, frac M2, frac T2, frac I2>
  constexpr Quantity_base<L - L2, M - M2, T - T2, I - I2> div(
      const Quantity_base<L2, M2, T2, I2> &x) {
    return {value / x.value};
  }

  constexpr Quantity_base<L, M, T, I> div(const double x) {
    return {value / x};
  }

  constexpr Quantity_base<L / 3, M / 3, T / 3, I / 3> abs() {
    return std::abs(value);
  }

  template <auto N>
  constexpr Quantity_base<L * N, M * N, T * N, I * N> pow() {
    return std::pow(value, N);
  }

  constexpr Quantity_base<L / 2, M / 2, T / 2, I / 2> sqrt() {
    return std::sqrt(value);
  }

  constexpr Quantity_base<L / 3, M / 3, T / 3, I / 3> cbrt() {
    return std::cbrt(value);
  }

  constexpr Quantity_base<L * 3, M * 3, T * 3, I * 3> cube() {
    return std::pow(value, 3);
  }

  // << operator
  template <frac LL, frac MM, frac TT, frac II>
  constexpr friend std::ostream &operator<<(
      std::ostream &os, const Quantity_base<LL, MM, TT, II> &frac);

  // + operator
  template <frac... D>
  constexpr friend Quantity_base<D...> operator+(Quantity_base<D...> x,
                                                 const Quantity_base<D...> &y);

  // - operator
  template <frac LL, frac MM, frac TT, frac II>
  constexpr friend Quantity_base<LL, MM, TT, II> operator-(
      Quantity_base<LL, MM, TT, II> x, const Quantity_base<LL, MM, TT, II> &y);
  template <frac LL, frac MM, frac TT, frac II>
  constexpr friend Quantity_base<LL, MM, TT, II> operator-(
      Quantity_base<LL, MM, TT, II> x);

  // * operator
  template <frac L1, frac M1, frac T1, frac I1,  //
            frac L2, frac M2, frac T2, frac I2>
  constexpr friend Quantity_base<L1 + L2, M1 + M2, T1 + T2, I1 + I2> operator*(
      Quantity_base<L1, M1, T1, I1> x, const Quantity_base<L2, M2, T2, I2> &y);
  template <frac L1, frac M1, frac T1, frac I1>
  constexpr friend Quantity_base<L1, M1, T1, I1> operator*(
      Quantity_base<L1, M1, T1, I1> x, const double &y);
  template <frac L1, frac M1, frac T1, frac I1>
  constexpr friend Quantity_base<L1, M1, T1, I1> operator*(
      const double &y, Quantity_base<L1, M1, T1, I1> x);

  // / operator
  template <frac L1, frac M1, frac T1, frac I1,  //
            frac L2, frac M2, frac T2, frac I2>
  constexpr friend Quantity_base<L1 - L2, M1 - M2, T1 - T2, I1 - I2> operator/(
      Quantity_base<L1, M1, T1, I1> x, const Quantity_base<L2, M2, T2, I2> &y);
  template <frac L1, frac M1, frac T1, frac I1>
  constexpr friend Quantity_base<L1, M1, T1, I1> operator/(
      Quantity_base<L1, M1, T1, I1> x, const double &y);
  template <frac L1, frac M1, frac T1, frac I1>
  constexpr friend Quantity_base<-L1, -M1, -T1, -I1> operator/(
      const double &y, Quantity_base<L1, M1, T1, I1> x);

  // == operator
};

template <frac L, frac M, frac T, frac I>
constexpr std::ostream &operator<<(std::ostream &os,
                                   const Quantity_base<L, M, T, I> &quantity) {
  os << "<Quantity:" << quantity.value << "<" << L << "," << M << "," << T
     << "," << I << ">>";
  return os;
}

template <frac... D>
constexpr Quantity_base<D...> operator+(Quantity_base<D...> x,
                                        const Quantity_base<D...> &y) {
  return x.add(y);
}

template <frac L, frac M, frac T, frac I>
constexpr Quantity_base<L, M, T, I> operator-(
    Quantity_base<L, M, T, I> x, const Quantity_base<L, M, T, I> &y) {
  return x.sub(y);
}
template <frac L, frac M, frac T, frac I>
constexpr Quantity_base<L, M, T, I> operator-(Quantity_base<L, M, T, I> x) {
  return {-x.value};
}

template <frac L1, frac M1, frac T1, frac I1,  //
          frac L2, frac M2, frac T2, frac I2>
constexpr Quantity_base<L1 + L2, M1 + M2, T1 + T2, I1 + I2> operator*(
    Quantity_base<L1, M1, T1, I1> x, const Quantity_base<L2, M2, T2, I2> &y) {
  return x.mul(y);
}
template <frac L1, frac M1, frac T1, frac I1>
constexpr Quantity_base<L1, M1, T1, I1> operator*(
    Quantity_base<L1, M1, T1, I1> x, const double &y) {
  return {x.value * y};
}
template <frac L1, frac M1, frac T1, frac I1>
constexpr Quantity_base<L1, M1, T1, I1> operator*(
    const double &y, Quantity_base<L1, M1, T1, I1> x) {
  return {x.value * y};
}

template <frac L1, frac M1, frac T1, frac I1,  //
          frac L2, frac M2, frac T2, frac I2>
constexpr Quantity_base<L1 - L2, M1 - M2, T1 - T2, I1 - I2> operator/(
    Quantity_base<L1, M1, T1, I1> x, const Quantity_base<L2, M2, T2, I2> &y) {
  return x.div(y);
}
template <frac L1, frac M1, frac T1, frac I1>
constexpr Quantity_base<L1, M1, T1, I1> operator/(
    Quantity_base<L1, M1, T1, I1> x, const double &y) {
  return {x.value / y};
}

// :TODO:
template <frac L, frac M, frac T, frac I>
constexpr Quantity_base<-L, -M, -T, -I> operator/(const double &y,
                                                  Quantity_base<L, M, T, I> x) {
  return {y / x.value};
}

template <frac L, frac M, frac T, frac I>
constexpr auto abs(Quantity_base<L, M, T, I> x) {
  return x.abs();
}

template <frac L, frac M, frac T, frac I>
constexpr auto sqrt(Quantity_base<L, M, T, I> x) {
  return x.sqrt();
}

template <frac L, frac M, frac T, frac I>
constexpr auto cbrt(Quantity_base<L, M, T, I> x) {
  return x.cbrt();
}

template <frac L, frac M, frac T, frac I>
constexpr auto cube(Quantity_base<L, M, T, I> x) {
  return x.cube();
}

template <auto N, frac L, frac M, frac T, frac I>
constexpr auto pow(Quantity_base<L, M, T, I> x) {
  return x.template pow<N>();
}

template <frac L, frac M, frac T, frac I>
constexpr Quantity_base<L, M, T, I> hypot(const Quantity_base<L, M, T, I> &x,
                                          const Quantity_base<L, M, T, I> &y) {
  return {std::hypot(x.value, y.value)};
}

template <frac L, frac M, frac T, frac I>
constexpr Quantity_base<L, M, T, I> hypot(const Quantity_base<L, M, T, I> &x,
                                          const Quantity_base<L, M, T, I> &y,
                                          const Quantity_base<L, M, T, I> &z) {
  return {std::hypot(x.value, y.value, z.value)};
}

}  // namespace Quantity
