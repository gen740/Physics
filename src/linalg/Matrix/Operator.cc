#include <memory>

#include "linalg/Matrix.h"

#ifdef PHYSICS_USE_MKL
#include <mkl_cblas.h>
#include <mkl_lapacke.h>
#else
#include <cblas.h>
#include <lapacke.h>
#endif

#include <iomanip>

namespace Linalg {

namespace {

auto matrix_precision = std::make_shared<int>(4);

}  // namespace

template <>
const std::shared_ptr<int> Matrix<double>::m_precision = matrix_precision;

template <>
const std::shared_ptr<int> Matrix<float>::m_precision = matrix_precision;

template <>
const std::shared_ptr<int> Matrix<std::complex<double>>::m_precision =
    matrix_precision;

template <>
const std::shared_ptr<int> Matrix<std::complex<float>>::m_precision =
    matrix_precision;

template <>
void Matrix<double>::set_precision(int precision) {
  *m_precision = precision;
}

template <>
void Matrix<float>::set_precision(int precision) {
  *m_precision = precision;
}

template <>
void Matrix<std::complex<double>>::set_precision(int precision) {
  *m_precision = precision;
}

template <>
void Matrix<std::complex<float>>::set_precision(int precision) {
  *m_precision = precision;
}

template <>
int Matrix<double>::get_precision() {
  return *m_precision;
}

template <>
int Matrix<float>::get_precision() {
  return *m_precision;
}

template <>
int Matrix<std::complex<double>>::get_precision() {
  return *m_precision;
}

template <>
int Matrix<std::complex<float>>::get_precision() {
  return *m_precision;
}

template <FloatingPointType T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &mat) {
  os << "Matrix " << mat.m_COL << "×" << mat.m_ROW << "\n";
  os << std::setprecision(*(mat.m_precision)) << std::scientific;
  for (size_t i = 1; i <= mat.m_COL; ++i) {
    os << "\t";
    for (size_t j = 1; j <= mat.m_ROW; ++j) {
      if (j == mat.m_ROW) {
        if (i == mat.m_COL) {
          os << mat(i, j);
          break;
        }
        os << mat(i, j) << "\n";
      } else {
        os << mat(i, j) << "\t";
      }
    }
  }
  os << std::defaultfloat;
  return os;
}

template std::ostream &operator<<(std::ostream &os, const Matrix<double> &mat);
template std::ostream &operator<<(std::ostream &os, const Matrix<float> &mat);
template std::ostream &operator<<(std::ostream &os, const Matrix<std::complex<double>> &mat);
template std::ostream &operator<<(std::ostream &os, const Matrix<std::complex<float>> &mat);

}  // namespace Linalg
