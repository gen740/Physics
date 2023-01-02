#pragma once

#include <complex>
#include <concepts>
#include <type_traits>

namespace Linalg {

template <class T>
concept FloatingPointType = std::same_as<T, double> || std::same_as<T, float> ||
                            std::same_as<T, std::complex<double>> ||
                            std::same_as<T, std::complex<float>>;

template <class>
struct is_complex : std::false_type {};

template <class T>
struct is_complex<std::complex<T>> : std::true_type {};

template <class T>
using is_complex_v = typename is_complex<T>::value;

template <class T>
struct base_type {
  using type = T;
};

template <class T>
struct base_type<std::complex<T>> {
  using type = T;
};

template <class T>
using base_type_t = typename base_type<T>::type;

}  // namespace Linalg
