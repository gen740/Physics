#pragma once

#include <concepts>
#include <iostream>

namespace autodiff {

template <int N = 2, class T = double>
  requires std::floating_point<T>
class dual {
 private:
  using ValType = T;
  std::array<T, N + 1> value_{0};

  dual(std::array<T, N + 1> value) : value_(value) {}
  dual(std::array<T, N + 1>&& value) : value_(std::move(value)) {}

 public:
  dual(T val) {
    value_[0] = val;
    value_[1] = 1;
  }

  auto value() { return value_; }

  friend dual<N, T> operator*(dual<N, T> a, dual<N, T> b) {
    std::array<T, N + 1> v{0};

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < i + 1; j++) {
        std::cout << i << ": " << j << "," << i - j << std::endl;
        std::cout << a.value_[j] * b.value_[i - j] << std::endl;
        v[i] += a.value_[j] * b.value_[i - j];
      }
    }

    return v;
  }
};

};  // namespace autodiff
