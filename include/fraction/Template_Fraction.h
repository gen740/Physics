#include <concepts>
#include <exception>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <stdexcept>

namespace Fraction {

template <int D, int N>
class tmp_frac {
 public:
  tmp_frac() = default;

 private:
  void add(const tmp_frac &a) {
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

  void sub(const tmp_frac &a) {
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

  void mul(tmp_frac a) {
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

  void div(tmp_frac a) {
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

  tmp_frac &operator=(tmp_frac &&frac) noexcept = default;
  tmp_frac &operator=(const tmp_frac &frac) = default;

  // friend std::ostream &operator<<(std::ostream &os, const frac<U> &frac);

  friend tmp_frac operator+(tmp_frac a, const tmp_frac &b);

  friend tmp_frac operator-(tmp_frac a, const tmp_frac &b);

  friend tmp_frac operator*(tmp_frac a, const tmp_frac &b);

  friend tmp_frac operator/(tmp_frac a, const tmp_frac &b);

 public:
  void operator++(int a) { add(1); }
  void operator--(int a) { sub(1); }

 private:
  // nume / deno
  int deno = D;  // 分母
  int nume = N;  // 分子
};

// template <integral T>
// std::ostream &operator<<(std::ostream &os, const frac<T> &frac) {
//   if (frac.deno == 1) {
//     os << "<frac:" << frac.nume << ">";
//   } else {
//     os << "<frac:" << frac.nume << "/" << frac.deno << ">";
//   }
//   return os;
// }
//
// template <integral U, integral V>
// frac<U> operator+(frac<U> a, const frac<V> &b) {
//   a.add(b);
//   return a;
// }
//
// template <integral U, integral V>
// frac<U> operator-(frac<U> a, const frac<V> &b) {
//   a.sub(b);
//   return a;
// }
//
// template <integral U, integral V>
// frac<U> operator*(frac<U> a, const frac<V> &b) {
//   a.mul(b);
//   return a;
// }
//
// template <integral U, integral V>
// frac<U> operator/(frac<U> a, const frac<V> &b) {
//   a.div(b);
//   return a;
// }

}  // namespace Fraction
