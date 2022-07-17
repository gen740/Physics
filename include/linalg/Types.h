#pragma once
#include <concepts>

namespace Linalg {

template <class T>
concept FloatingPointType = std::is_floating_point_v<T>;

}
