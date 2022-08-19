#include <compare>
#include <concepts>
#include <exception>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <stdexcept>

namespace Fraction {

template <class T>
concept integral = std::is_integral_v<T>;

template <integral T = int_fast32_t>
struct frac {
 public:
  // nume / deno
  T nume{1};  // 分子
  T deno{1};  // 分母
 public:
  frac() = default;
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
  constexpr frac(T n) : deno(1), nume(n) {}
  frac(frac const &frac) = default;
  frac(frac &&frac) = default;
  ~frac() = default;

  constexpr T get_numerator() const { return nume; }
  constexpr T get_denominator() const { return deno; }

  std::string to_str() const {
    if (deno == 1) {
      return std::to_string(nume);
    } else if (nume == 0) {
      return "0";
    } else {
      return (std::to_string(nume) + "/" + std::to_string(deno));
    }
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

  template <integral U>
  constexpr friend std::ostream &operator<<(std::ostream &os,
                                            const frac<U> &frac);

  template <integral U, integral V>
  constexpr friend frac<U> operator+(frac<U> a, const frac<V> &b);

  template <integral U, integral V>
  constexpr friend frac<U> operator-(frac<U> a, const frac<V> &b);

  template <integral U>
  constexpr friend frac<U> operator-(frac<U> a);

  template <integral U, integral V>
  constexpr friend frac<U> operator*(frac<U> a, const frac<V> &b);

  template <integral U, integral V>
  constexpr friend frac<U> operator*(frac<U> a, const V &b);

  template <integral U, integral V>
  constexpr friend frac<U> operator/(frac<U> a, const frac<V> &b);

  template <integral U, integral V>
  constexpr friend frac<U> operator/(frac<U> a, const V &b);

  // template <integral U, integral V>
  // constexpr friend int operator<=>(frac<U>, frac<V>);

  template <integral U, integral V>
  constexpr friend bool operator==(frac<U>, frac<V>);

 public:
  template <integral U>
  constexpr auto operator<=>(const frac<U> &x) const {
    return this->eval() <=> x.eval();
  }

  constexpr void operator++(int a) { add(1); }
  constexpr void operator--(int a) { sub(1); }
};

template <integral T>
constexpr std::ostream &operator<<(std::ostream &os, const frac<T> &frac) {
  if (frac.deno == 1) {
    os << "<frac:" << frac.nume << ">";
  } else {
    os << "<frac:" << frac.nume << "/" << frac.deno << ">";
  }
  return os;
}

template <integral U, integral V>
constexpr frac<U> operator+(frac<U> a, const frac<V> &b) {
  a.add(b);
  return a;
}

template <integral U, integral V>
constexpr frac<U> operator-(frac<U> a, const frac<V> &b) {
  a.sub(b);
  return a;
}

template <integral U>
constexpr frac<U> operator-(frac<U> a) {
  return {-a.nume, a.deno};
}

template <integral U, integral V>
constexpr frac<U> operator*(frac<U> a, const frac<V> &b) {
  a.mul(b);
  return a;
}

template <integral U, integral V>
constexpr frac<U> operator*(frac<U> a, const V &b) {
  a.mul(b);
  return a;
}

template <integral U, integral V>
constexpr frac<U> operator/(frac<U> a, const frac<V> &b) {
  a.div(b);
  return a;
}

template <integral U, integral V>
constexpr frac<U> operator/(frac<U> a, const V &b) {
  a.div(b);
  return a;
}

template <integral U, integral V>
constexpr bool operator==(frac<U> x, frac<V> y) {
  return (x.deno == y.deno) && (x.nume == y.nume);
}

}  // namespace Fraction
