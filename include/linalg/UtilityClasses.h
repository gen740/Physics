#pragma once
#include <vector>

namespace Linalg {

struct LU_status {
  int status{};
  std::vector<int> ipiv;
  explicit operator int() const { return status; }
  explicit LU_status(int size) : ipiv(size) {}
};

}  // namespace Linalg
