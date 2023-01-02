#pragma once

#include <concepts>
#include <iostream>

namespace autodiff {

template <class T>
concept Dual = requires(T& x) {
                 { x* x } -> std::same_as<T>;
               };

constexpr int_fast32_t factorial_(int_fast32_t n) {
  int_fast32_t ret{1};
  for (int i = 1; i <= n; i++) {
    ret *= i;
  }
  return ret;
}

constexpr int_fast32_t permutation_(int_fast32_t n, int_fast32_t m) {
  int_fast32_t den{1};
  int_fast32_t num{1};
  for (int i = 0; i < m; i++) {
    num *= n - i;
    den *= (i + 1);
  }
  return num / den;
}

template <int N = 2, class T = double>
  requires std::floating_point<T>
class dual {
 private:
  using ValType = T;
  std::array<T, N + 1> value_{0};

  dual(const std::array<T, N + 1>& value) : value_(value) {}
  dual(std::array<T, N + 1>&& value) : value_(std::move(value)) {}

 public:
  dual(T val) {
    value_[0] = val;
    value_[1] = 1;
  }

  auto value() { return value_; }

  friend dual<N, T> operator*(dual<N, T> a, dual<N, T> b) {
    std::array<T, N + 1> v{0};

    for (int i = 0; i < N + 1; i++) {
      for (int j = 0; j < i + 1; j++) {
        v[i] += permutation_(i, i - j) * a.value_[j] * b.value_[i - j];
      }
    }

    return std::move(v);
  }
};

};  // namespace autodiff
