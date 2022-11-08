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
#include <iomanip>
#include <iostream>

namespace Linalg {

namespace {

int matrix_precision = 4;

}  // namespace

template <>
int *const Matrix<double>::m_precision = &matrix_precision;

template <>
int *const Matrix<float>::m_precision = &matrix_precision;

template <>
int *const Matrix<_Complex double>::m_precision = &matrix_precision;

template <>
int *const Matrix<_Complex float>::m_precision = &matrix_precision;

template <FloatingPointType T>
Matrix<T>::Matrix(const Matrix<T> &mat, int col, int row)
    : m_COL(col), m_ROW(row) {
  if (mat.m_ROW * mat.m_COL != col * row) {
    throw std::runtime_error("配列の大きさが違います");
  }
  m_data = mat.m_data;
}

template <FloatingPointType T>
Matrix<T>::Matrix(Matrix<T> &&mat, int col, int row) : m_COL(col), m_ROW(row) {
  if (mat.m_ROW * mat.m_COL != col * row) {
    throw std::runtime_error("配列の大きさが違います");
  }
  m_data = std::move(mat.m_data);
}

template <FloatingPointType T>
Matrix<T>::Matrix(const Vector<T> &vec, int col, int row)
    : m_COL(col), m_ROW(row) {
  if (vec.m_SIZE != col * row) {
    throw std::runtime_error("配列の大きさが違います");
  }
  m_data = vec.m_data;
}

template <FloatingPointType T>
Matrix<T>::Matrix(std::vector<std::vector<T>> vec)
    : m_COL(vec.size()), m_ROW(vec[0].size()) {
  for (int i = 1; i < m_COL; ++i) {
    if (m_ROW != vec[i].size()) {
      throw std::runtime_error("Matrix Constructor recieve none matrix vector");
    }
  }
  // m_data.reserve(m_COL * m_ROW, 0.);
  m_data.resize(m_COL * m_ROW);
  for (int i = 0; i < m_ROW; ++i) {
    for (int j = 0; j < m_COL; ++j) {
      m_data[i * m_COL + j] = vec[j][i];
    }
  }
}

template <>
void Matrix<double>::set_precision(int precision) {
  *m_precision = precision;
}

template <>
void Matrix<float>::set_precision(int precision) {
  *m_precision = precision;
}

template <>
void Matrix<_Complex double>::set_precision(int precision) {
  *m_precision = precision;
}

template <>
void Matrix<_Complex float>::set_precision(int precision) {
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
int Matrix<double _Complex>::get_precision() {
  return *m_precision;
}

template <>
int Matrix<float _Complex>::get_precision() {
  return *m_precision;
}

template <FloatingPointType T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &mat) {
  os << "Matrix " << mat.m_COL << "×" << mat.m_ROW << "\n";
  os << std::setprecision(*(mat.m_precision)) << std::scientific;
  for (int i = 1; i <= mat.m_COL; ++i) {
    os << "\t";
    for (int j = 1; j <= mat.m_ROW; ++j) {
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

template <FloatingPointType T>
Matrix<T> Matrix<T>::Diag(Vector<T> vec, int col, int row) {
  if (col == -1) {
    col = vec.size();
  }
  if (row == -1) {
    row = vec.size();
  }
  if (vec.size() > col && vec.size() > row) {
    throw std::runtime_error(
        "Not Correct args, col and row should be larger than vector size");
  }
  Matrix<T> ret(col, row);
  for (int i = 0; i < vec.size(); ++i) {
    ret[i][i] = vec[i];
  }
  return ret;
}

template <>
void Matrix<>::save(const char *filename, const char delimeter,
                    bool is_scientific) {
  std::ofstream file(filename);
  if (is_scientific) {
    file << std::scientific;
  }
  for (int i = 1; i <= m_COL; ++i) {
    for (int j = 1; j <= m_ROW; ++j) {
      if (j == m_ROW) {
        if (i == m_COL) {
          file << (*this)(i, j);
          break;
        }
        file << (*this)(i, j) << '\n';
      } else {
        file << (*this)(i, j) << delimeter;
      }
    }
  }
  file << '\n';
}

template <>
LU_status Matrix<double>::lu() {
  LU_status status(m_ROW);
  status.status = LAPACKE_dgetrf(LAPACK_COL_MAJOR, m_COL, m_ROW, m_data.data(),
                                 m_COL, status.ipiv.data());
  return status;
}

template <>
LU_status Matrix<float>::lu() {
  LU_status status(m_ROW);
  status.status = LAPACKE_sgetrf(LAPACK_COL_MAJOR, m_COL, m_ROW, m_data.data(),
                                 m_COL, status.ipiv.data());
  return status;
}

template <>
LU_status Matrix<_Complex double>::lu() {
  LU_status status(m_ROW);
  status.status = LAPACKE_zgetrf(LAPACK_COL_MAJOR, m_COL, m_ROW, m_data.data(),
                                 m_COL, status.ipiv.data());
  return status;
}

template <>
LU_status Matrix<_Complex float>::lu() {
  LU_status status(m_ROW);
  status.status = LAPACKE_cgetrf(LAPACK_COL_MAJOR, m_COL, m_ROW, m_data.data(),
                                 m_COL, status.ipiv.data());
  return status;
}

template <>
int Matrix<double>::svd(Vector<double> &s, Matrix<double> &u,
                        Matrix<double> &v) {
  Vector<double> superb(std::min(m_ROW, m_COL));
  u.reshape(m_COL, m_COL);
  v.reshape(m_ROW, m_ROW);
  s.reshape(std::min(m_COL, m_ROW));
  return LAPACKE_dgesvd(LAPACK_COL_MAJOR, 'A', 'A', m_COL, m_ROW, *this, m_COL,
                        s, u, m_COL, v, m_ROW, superb);
}

template <>
int Matrix<float>::svd(Vector<float> &s, Matrix<float> &u, Matrix<float> &v) {
  Vector<float> superb(std::min(m_ROW, m_COL));
  u.reshape(m_COL, m_COL);
  v.reshape(m_ROW, m_ROW);
  s.reshape(std::min(m_COL, m_ROW));
  return LAPACKE_sgesvd(LAPACK_COL_MAJOR, 'A', 'A', m_COL, m_ROW, *this, m_COL,
                        s, u, m_COL, v, m_ROW, superb);
}

template <>
int Matrix<_Complex double>::svd(Vector<double> &s, Matrix<_Complex double> &u,
                                 Matrix<_Complex double> &v) {
  Vector<double> superb(std::min(m_ROW, m_COL));
  u.reshape(m_COL, m_COL);
  v.reshape(m_ROW, m_ROW);
  s.reshape(std::min(m_COL, m_ROW));
  return LAPACKE_zgesvd(LAPACK_COL_MAJOR, 'A', 'A', m_COL, m_ROW, *this, m_COL,
                        s, u, m_COL, v, m_ROW, superb);
}

template <>
int Matrix<_Complex float>::svd(Vector<float> &s, Matrix<_Complex float> &u,
                                Matrix<_Complex float> &v) {
  Vector<float> superb(std::min(m_ROW, m_COL));
  u.reshape(m_COL, m_COL);
  v.reshape(m_ROW, m_ROW);
  s.reshape(std::min(m_COL, m_ROW));
  return LAPACKE_cgesvd(LAPACK_COL_MAJOR, 'A', 'A', m_COL, m_ROW, *this, m_COL,
                        s, u, m_COL, v, m_ROW, superb);
}

template <FloatingPointType T>
T Matrix<T>::det() {
  T det(1.);
  Matrix m = *this;
  if (m.m_ROW != m.m_COL) {
    throw std::runtime_error("Matrix is not squre");
  }
  auto status = m.lu();
  for (int i = 0; i < m.m_COL; ++i) {
    det *= m[i][i];
    if (status.ipiv.at(i) != i + 1) {
      det *= -1;
    }
  }
  return det;
}

template std::ostream &operator<<(std::ostream &os, const Matrix<double> &mat);
template std::ostream &operator<<(std::ostream &os, const Matrix<float> &mat);
template std::ostream &operator<<(std::ostream &os,
                                  const Matrix<_Complex double> &mat);
template std::ostream &operator<<(std::ostream &os,
                                  const Matrix<_Complex float> &mat);

template <FloatingPointType T>
T &Matrix<T>::operator()(int col, int row) {
  if (1 <= col && col <= m_COL && 1 <= row && row <= m_ROW) {
    return m_data[(row - 1) * m_COL + (col - 1)];
  }
  throw std::runtime_error("Index out of range");
}

template <FloatingPointType T>
T Matrix<T>::operator()(int col, int row) const {
  if (1 <= col && col <= m_COL && 1 <= row && row <= m_ROW) {
    return m_data[(row - 1) * m_COL + (col - 1)];
  }
  throw std::runtime_error("Index out of range");
}

template <>
Matrix<double> Matrix<double>::operator*(Matrix<double> mat) {
  auto mat_shape = mat.shape();
  if (m_ROW != mat_shape[0]) {
    throw std::runtime_error("Cannot Multiply matrix");
  }
  Matrix<double> ret(m_COL, mat_shape[1]);
  cblas_dgemm(CBLAS_ORDER::CblasColMajor, CBLAS_TRANSPOSE::CblasNoTrans,
              CBLAS_TRANSPOSE::CblasNoTrans, m_COL, mat_shape[1], m_ROW, 1,
              *this, m_COL, mat, mat_shape[0], 1, ret, m_COL);
  return ret;
}

template <>
Matrix<float> Matrix<float>::operator*(Matrix<float> mat) {
  auto mat_shape = mat.shape();
  if (m_ROW != mat_shape[0]) {
    throw std::runtime_error("Cannot Multiply matrix");
  }
  Matrix<float> ret(m_COL, mat_shape[1]);
  cblas_sgemm(CBLAS_ORDER::CblasColMajor, CBLAS_TRANSPOSE::CblasNoTrans,
              CBLAS_TRANSPOSE::CblasNoTrans, m_COL, mat_shape[1], m_ROW, 1,
              *this, m_COL, mat, mat_shape[0], 1, ret, m_COL);
  return ret;
}

template <>
Matrix<_Complex double> Matrix<_Complex double>::operator*(
    Matrix<_Complex double> mat) {
  auto mat_shape = mat.shape();
  if (m_ROW != mat_shape[0]) {
    throw std::runtime_error("Cannot Multiply matrix");
  }

  Matrix<_Complex double> ret(m_COL, mat_shape[1]);
  _Complex double alpha = 1;
  _Complex double beta = 1;
  cblas_zgemm(CBLAS_ORDER::CblasColMajor, CBLAS_TRANSPOSE::CblasNoTrans,
              CBLAS_TRANSPOSE::CblasNoTrans, m_COL, mat_shape[1], m_ROW, &alpha,
              *this, m_COL, mat, mat_shape[0], &beta, ret, m_COL);
  return ret;
}

template <>
Matrix<_Complex float> Matrix<_Complex float>::operator*(
    Matrix<_Complex float> mat) {
  auto mat_shape = mat.shape();
  if (m_ROW != mat_shape[0]) {
    throw std::runtime_error("Cannot Multiply matrix");
  }
  Matrix<_Complex float> ret(m_COL, mat_shape[1]);
  _Complex float alpha = 1;
  _Complex float beta = 1;
  cblas_cgemm(CBLAS_ORDER::CblasColMajor, CBLAS_TRANSPOSE::CblasNoTrans,
              CBLAS_TRANSPOSE::CblasNoTrans, m_COL, mat_shape[1], m_ROW, &alpha,
              *this, m_COL, mat, mat_shape[0], &beta, ret, m_COL);
  return ret;
}

template class Matrix<double>;
template class Matrix<_Complex double>;
template class Matrix<float>;
template class Matrix<_Complex float>;

}  // namespace Linalg
