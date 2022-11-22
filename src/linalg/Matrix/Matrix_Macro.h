#pragma once

#define Implement_Matrix_LAPACK_func(func) \
  func(double, d);                         \
  func(float, s);                          \
  func(std::complex<double>, z);           \
  func(std::complex<float>, c);
