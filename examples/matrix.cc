#include <linalg/Matrix.h>

#include <cassert>
#include <iostream>

int main() {
  Linalg::Matrix<> m({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
  std::cout << m << std::endl;
  // 1-index Matrix access
  m(3, 3) = 11;           // write
  assert(m(3, 3) == 11);  // read
  std::cout << m << std::endl;

  // or C style matrix access
  m[2][2] = 10;           // write
  assert(m[2][2] == 10);  // read

  // Matrix has some basic linear algebra functions.
  std::cout << m.det() << std::endl;
  return 0;
}
