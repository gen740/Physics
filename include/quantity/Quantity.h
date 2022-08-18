#pragma once
#include <fraction/Fraction.h>

template <Fraction::frac L = 0, Fraction::frac M = 0,  //
          Fraction::frac T = 0, Fraction::frac I = 0>
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

  // Allow same dimenthion sub / div
  constexpr Quantity_base<L, M, T, I> add(const Quantity_base<L, M, T, I> &x) {
    return {value + x.value};
  }

  constexpr Quantity_base<L, M, T, I> sub(const Quantity_base<L, M, T, I> &x) {
    return {value - x.value};
  }

  template <Fraction::frac L2, Fraction::frac M2,  //
            Fraction::frac T2, Fraction::frac I2>
  constexpr Quantity_base<L + L2, M + M2, T + T2, I + I2> mul(
      const Quantity_base<L2, M2, T2, I2> &x) {
    return {value * x.value};
  }

  constexpr Quantity_base<L, M, T, I> mul(const double x) {
    return {value * x};
  }

  template <Fraction::frac L2, Fraction::frac M2,  //
            Fraction::frac T2, Fraction::frac I2>
  constexpr Quantity_base<L - L2, M - M2, T - T2, I - I2> div(
      const Quantity_base<L2, M2, T2, I2> &x) {
    return std::sqrt(value / x.value);
  }

  constexpr Quantity_base<L, M, T, I> div(const double x) {
    return {value / x};
  }

  template <auto N>
  constexpr Quantity_base<L * N, M * N, T * N, I * N> pow() {
    return std::pow(value, N);
  }

  constexpr Quantity_base<L / 2, M / 2, T / 2, I / 2> sqrt() {
    return std::sqrt(value);
  }

  constexpr Fraction::frac<> get_L() { return L; }
  constexpr Fraction::frac<> get_M() { return M; }
  constexpr Fraction::frac<> get_T() { return T; }
  constexpr Fraction::frac<> get_I() { return I; }

  template <Fraction::frac LL, Fraction::frac MM,  //
            Fraction::frac TT, Fraction::frac II>
  constexpr friend std::ostream &operator<<(
      std::ostream &os, const Quantity_base<LL, MM, TT, II> &frac);

  template <Fraction::frac LL, Fraction::frac MM,  //
            Fraction::frac TT, Fraction::frac II>
  constexpr friend Quantity_base<LL, MM, TT, II> operator+(
      Quantity_base<LL, MM, TT, II> x, const Quantity_base<LL, MM, TT, II> &y);

  template <Fraction::frac LL, Fraction::frac MM,  //
            Fraction::frac TT, Fraction::frac II>
  constexpr friend Quantity_base<LL, MM, TT, II> operator-(
      Quantity_base<LL, MM, TT, II> x, const Quantity_base<LL, MM, TT, II> &y);

  template <Fraction::frac L1, Fraction::frac M1,  //
            Fraction::frac T1, Fraction::frac I1,  //
            Fraction::frac L2, Fraction::frac M2,  //
            Fraction::frac T2, Fraction::frac I2>
  constexpr friend Quantity_base<L1 + L2, M1 + M2, T1 + T2, I1 + I2> operator*(
      Quantity_base<L1, M1, T1, I1> x, const Quantity_base<L2, M2, T2, I2> &y);

  template <Fraction::frac L1, Fraction::frac M1,  //
            Fraction::frac T1, Fraction::frac I1,  //
            Fraction::frac L2, Fraction::frac M2,  //
            Fraction::frac T2, Fraction::frac I2>
  constexpr friend Quantity_base<L1 + L2, M1 + M2, T1 + T2, I1 + I2> operator/(
      Quantity_base<L1, M1, T1, I1> x, const Quantity_base<L2, M2, T2, I2> &y);
};

template <Fraction::frac L, Fraction::frac M,  //
          Fraction::frac T, Fraction::frac I>
constexpr std::ostream &operator<<(std::ostream &os,
                                   const Quantity_base<L, M, T, I> &quantity) {
  os << "<Quantity:" << quantity.value << "<" << L << "," << M << "," << T
     << "," << I << ">>";
  return os;
}

template <Fraction::frac L, Fraction::frac M,  //
          Fraction::frac T, Fraction::frac I>
constexpr Quantity_base<L, M, T, I> operator+(
    Quantity_base<L, M, T, I> x, const Quantity_base<L, M, T, I> &y) {
  return x.add(y);
}

template <Fraction::frac L, Fraction::frac M,  //
          Fraction::frac T, Fraction::frac I>
constexpr Quantity_base<L, M, T, I> operator-(
    Quantity_base<L, M, T, I> x, const Quantity_base<L, M, T, I> &y) {
  return x.sub(y);
}

template <Fraction::frac L1, Fraction::frac M1,  //
          Fraction::frac T1, Fraction::frac I1,  //
          Fraction::frac L2, Fraction::frac M2,  //
          Fraction::frac T2, Fraction::frac I2>
constexpr Quantity_base<L1 + L2, M1 + M2, T1 + T2, I1 + I2> operator*(
    Quantity_base<L1, M1, T1, I1> x, const Quantity_base<L2, M2, T2, I2> &y) {
  return x.mul(y);
}

template <Fraction::frac L1, Fraction::frac M1,  //
          Fraction::frac T1, Fraction::frac I1,  //
          Fraction::frac L2, Fraction::frac M2,  //
          Fraction::frac T2, Fraction::frac I2>
constexpr Quantity_base<L1 + L2, M1 + M2, T1 + T2, I1 + I2> operator/(
    Quantity_base<L1, M1, T1, I1> x, const Quantity_base<L2, M2, T2, I2> &y) {
  return x.div(y);
}
