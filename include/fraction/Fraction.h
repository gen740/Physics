#pragma once

#include <compare>
#include <concepts>
#include <exception>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <stdexcept>

namespace Fraction {

template <std::signed_integral T = int_fast32_t>
struct frac {
 public:
  // nume / deno
  T nume{0};  // 分子
  T deno{1};  // 分母

  constexpr frac(T nume, T deno) {
    auto r = std::gcd(deno, nume);
    this->deno = deno / r;
    this->nume = nume / r;
    if (this->deno == 0) {
      throw std::runtime_error("Fraction cannot devide by zero");
    }
    if (this->deno < 0) {
      this->nume *= -1;
      this->deno *= -1;
    }
  };
  constexpr frac(T n) : nume(n), deno(1) {}

  frac() = default;
  frac(frac const &frac) = default;
  frac(frac &&frac) noexcept = default;
  ~frac() = default;

  constexpr T get_numerator() const { return nume; }
  constexpr T get_denominator() const { return deno; }

  [[nodiscard]] std::string to_str() const {
    if (deno == 1) {
      return std::to_string(nume);
    }
    if (nume == 0) {
      return "0";
    }
    return (std::to_string(nume) + "/" + std::to_string(deno));
  }

 private:
  constexpr void add(const frac &a) {
    auto ab_ba = a.deno * nume + deno * a.nume;
    auto ab = a.deno * deno;
    auto r = std::gcd(ab_ba, ab);
    nume = ab_ba / r;
    deno = ab / r;
    if (deno < 0) {
      nume *= -1;
      deno *= -1;
    }
  }

  constexpr void sub(const frac &a) {
    auto ab_ba = a.deno * nume - deno * a.nume;
    auto ab = a.deno * deno;
    auto r = std::gcd(ab_ba, ab);
    nume = ab_ba / r;
    deno = ab / r;
    if (deno < 0) {
      nume *= -1;
      deno *= -1;
    }
  }

  constexpr void mul(frac a) {
    deno *= a.deno;
    nume *= a.nume;
    auto r = std::gcd(deno, nume);
    nume /= r;
    deno /= r;
    if (deno < 0) {
      nume *= -1;
      deno *= -1;
    }
  }

  constexpr void div(frac a) {
    deno *= a.nume;
    nume *= a.deno;
    auto r = std::gcd(deno, nume);
    nume /= r;
    deno /= r;
    if (deno < 0) {
      nume *= -1;
      deno *= -1;
    }
  }

 public:
  [[nodiscard]] double eval() const { return static_cast<double>(nume) / deno; }

  frac &operator=(frac &&frac) noexcept = default;
  frac &operator=(const frac &frac) = default;

  template <std::signed_integral U>
  constexpr friend std::ostream &operator<<(std::ostream &os,
                                            const frac<U> &frac) {
    if (frac.deno == 1) {
      os << "<frac:" << frac.nume << ">";
    } else {
      os << "<frac:" << frac.nume << "/" << frac.deno << ">";
    }
    return os;
  }

  constexpr friend frac<T> operator+(frac<T> a, const frac<T> &b) {
    a.add(b);
    return a;
  }

  template <std::integral U>
  constexpr friend frac<T> operator+(frac<T> a, const U &b) {
    a.add(b);
    return a;
  }

  template <std::integral U>
  constexpr friend frac<U> operator+(const U &a, const frac<T> &b) {
    return b + a;
  }

  constexpr friend frac<T> operator-(frac<T> a, const frac<T> &b) {
    a.sub(b);
    return a;
  }

  template <std::integral U>
  constexpr friend frac<T> operator-(frac<T> a, const U &b) {
    a.sub(b);
    return a;
  }

  template <std::integral U>
  constexpr friend frac<U> operator-(const U &a, const frac<T> &b) {
    return -b + a;
  }

  constexpr friend frac<T> operator-(frac<T> a) { return {-a.nume, a.deno}; }

  constexpr friend frac<T> operator*(frac<T> a, const frac<T> &b) {
    a.mul(b);
    return a;
  }

  template <std::integral U>
  constexpr friend frac<T> operator*(frac<T> a, const U &b) {
    a.mul(b);
    return a;
  }

  template <std::integral U>
  constexpr friend frac<U> operator*(const U &a, const frac<T> &b) {
    return b * a;
  }

  constexpr friend frac<T> operator/(frac<T> a, const frac<T> &b) {
    a.div(b);
    return a;
  }

  template <std::integral U>
  constexpr friend frac<T> operator/(frac<T> a, const U &b) {
    a.div(b);
    return a;
  }

  template <std::integral U>
  constexpr friend frac<T> operator/(const U &a, const frac<T> &b) {
    frac<T> ret(a);
    ret.div(b);
    return ret;
  }

  template <std::signed_integral U>
  constexpr friend bool operator==(frac<T> x, frac<U> y) {
    return (x.deno == y.deno) && (x.nume == y.nume);
  }

  template <std::signed_integral U>
  constexpr auto operator<=>(const frac<U> &x) const {
    return this->eval() <=> x.eval();
  }

  template <std::signed_integral U>
  constexpr operator frac<U>() const {
    return frac<U>(deno, nume);
  }

  constexpr void operator++([[maybe_unused]] int a) { add(1); }
  constexpr void operator--([[maybe_unused]] int a) { sub(1); }
};

}  // namespace Fraction
