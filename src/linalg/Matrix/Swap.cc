#include <optional>

#include "linalg/Matrix.h"

#ifdef PHYSICS_USE_MKL
#include <mkl_cblas.h>
#include <mkl_lapacke.h>
#else
#include <cblas.h>
#include <lapacke.h>
#endif

#include <algorithm>
#include <exception>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>

namespace Linalg {

template <>
void Matrix<double>::swap(size_t i, size_t j) {
  if (i > m_ROW) {
    throw std::runtime_error("i is greater than row size");
  }
  if (j > m_ROW) {
    throw std::runtime_error("j is greater than row size");
  }
  if (i == j) {
    return;
  }
  cblas_dswap(static_cast<blasint>(m_COL),  //
              this[i],                      //
              1,                            //
              this[j],                      //
              1                             //

  );
}

template <>
void Matrix<float>::swap(size_t i, size_t j) {
  if (i > m_ROW) {
    throw std::runtime_error("i is greater than row size");
  }
  if (j > m_ROW) {
    throw std::runtime_error("j is greater than row size");
  }
  if (i == j) {
    return;
  }
  cblas_sswap(static_cast<blasint>(m_COL),  //
              this[i],                      //
              1,                            //
              this[j],                      //
              1                             //

  );
}

template <>
void Matrix<std::complex<double>>::swap(size_t i, size_t j) {
  if (i > m_ROW) {
    throw std::runtime_error("i is greater than row size");
  }
  if (j > m_ROW) {
    throw std::runtime_error("j is greater than row size");
  }
  if (i == j) {
    return;
  }
  cblas_zswap(static_cast<blasint>(m_COL),  //
              this[i],                      //
              1,                            //
              this[j],                      //
              1                             //

  );
}

template <>
void Matrix<std::complex<float>>::swap(size_t i, size_t j) {
  if (i > m_ROW) {
    throw std::runtime_error("i is greater than row size");
  }
  if (j > m_ROW) {
    throw std::runtime_error("j is greater than row size");
  }
  if (i == j) {
    return;
  }
  cblas_cswap(static_cast<blasint>(m_COL),  //
              this[i],                      //
              1,                            //
              this[j],                      //
              1                             //

  );
}

}  // namespace Linalg
