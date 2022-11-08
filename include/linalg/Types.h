#pragma once
#include <concepts>

namespace Linalg {

template <class T>
concept FloatingPointType =
    std::same_as<T, float> || std::same_as<T, double> ||
    std::same_as<T, double _Complex> || std::same_as<T, float _Complex>;

// template <class T>
// concept FloatingPointType =
//     std::same_as<T, float> && std::same_as<T, double> &&
//     std::same_as<T, double _Complex> && std::same_as<T, float _Complex>;

}  // namespace Linalg
