#pragma once
#include <complex>
#include <concepts>
#include <type_traits>

namespace Linalg {

template <class T>
concept FloatingPointType = std::same_as<T, double> || std::same_as<T, float> ||
                            std::same_as<T, std::complex<double>> ||
                            std::same_as<T, std::complex<float>>;

template <class T>
struct is_complex {
  static constexpr bool value = std::is_same_v<T, std::complex<double>> ||
                                std::is_same_v<T, std::complex<float>>;
};

template <class T>
using is_complex_v = typename is_complex<T>::value;

// template <class T>
// struct get_basetype {
//
// }

}  // namespace Linalg
