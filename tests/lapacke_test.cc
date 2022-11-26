#include <gtest/gtest.h>

#ifdef PHYSICS_USE_MKL
#include <mkl_lapacke.h>
#else
#include <lapacke.h>
#endif

#include <linalg/Matrix.h>

#include <array>

using Linalg::Matrix;

TEST(Lapacke, Basic) {
  Matrix<>::set_precision(5);
  Matrix<> m(3, 3);
  m.set_precision(5);
  m(1, 1) = 1;
  m(1, 2) = 2;
  m(1, 3) = 3;
  m(2, 1) = 4;
  m(2, 2) = 5;
  m(2, 3) = 6;
  m(3, 1) = 7;
  m(3, 2) = 8;
  m(3, 3) = 10;
  // Matrix<float>::set_precision(10);
  std::array<int, 3> ipiv;
  // Matrix<2, 2> n({{1, 2}, {1, 2}});
  LAPACKE_dgetrf(LAPACK_COL_MAJOR, 3, 3, m, 3, ipiv.data());
}
