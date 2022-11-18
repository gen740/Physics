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

template <FloatingPointType T>
T &Matrix<T>::operator()(size_t col, size_t row) {
  if (1 <= col && col <= m_COL && 1 <= row && row <= m_ROW) {
    return m_data[(row - 1) * m_COL + (col - 1)];
  }
  throw std::runtime_error("Index out of range");
}

template <FloatingPointType T>
T Matrix<T>::operator()(size_t col, size_t row) const {
  if (1 <= col && col <= m_COL && 1 <= row && row <= m_ROW) {
    return m_data[(row - 1) * m_COL + (col - 1)];
  }
  throw std::runtime_error("Index out of range");
}

template <>
Matrix<double> Matrix<double>::operator*(Matrix<double> mat) {
  if (m_ROW != mat.m_COL) {
    throw std::runtime_error("Cannot Multiply matrix");
  }
  Matrix<double> ret(m_COL, mat.m_ROW);
  cblas_dgemm(                        //
      CBLAS_ORDER::CblasColMajor,     //
      CBLAS_TRANSPOSE::CblasNoTrans,  //
      CBLAS_TRANSPOSE::CblasNoTrans,  //
      static_cast<int>(m_COL),        //
      static_cast<int>(mat.m_ROW),    //
      static_cast<int>(m_ROW),        //
      1,                              //
      *this,                          //
      static_cast<int>(m_COL),        //
      mat,                            //
      static_cast<int>(mat.m_COL),    //
      1,                              //
      ret,                            //
      static_cast<int>(m_COL)         //
  );
  return ret;
}

template <>
Matrix<float> Matrix<float>::operator*(Matrix<float> mat) {
  if (m_ROW != mat.m_COL) {
    throw std::runtime_error("Cannot Multiply matrix");
  }
  Matrix<float> ret(m_COL, mat.m_ROW);

  cblas_sgemm(                        //
      CBLAS_ORDER::CblasColMajor,     //
      CBLAS_TRANSPOSE::CblasNoTrans,  //
      CBLAS_TRANSPOSE::CblasNoTrans,  //
      static_cast<int>(m_COL),        //
      static_cast<int>(mat.m_ROW),    //
      static_cast<int>(m_ROW),        //
      1,                              //
      *this,                          //
      static_cast<int>(m_COL),        //
      mat,                            //
      static_cast<int>(mat.m_COL),    //
      1,                              //
      ret,                            //
      static_cast<int>(m_COL)         //
  );
  return ret;
}

template <>
Matrix<std::complex<double>> Matrix<std::complex<double>>::operator*(
    Matrix<std::complex<double>> mat) {
  if (m_ROW != mat.m_COL) {
    throw std::runtime_error("Cannot Multiply matrix");
  }

  Matrix<std::complex<double>> ret(m_COL, mat.m_ROW);
  std::complex<double> alpha = 1;
  std::complex<double> beta = 1;
  cblas_zgemm(                        //
      CBLAS_ORDER::CblasColMajor,     //
      CBLAS_TRANSPOSE::CblasNoTrans,  //
      CBLAS_TRANSPOSE::CblasNoTrans,  //
      static_cast<int>(m_COL),        //
      static_cast<int>(mat.m_ROW),    //
      static_cast<int>(m_ROW),        //
      &alpha,                         //
      *this,                          //
      static_cast<int>(m_COL),        //
      mat,                            //
      static_cast<int>(mat.m_COL),    //
      &beta,                          //
      ret,                            //
      static_cast<int>(m_COL)         //
  );
  return ret;
}

template <>
Matrix<std::complex<float>> Matrix<std::complex<float>>::operator*(
    Matrix<std::complex<float>> mat) {
  if (m_ROW != mat.m_COL) {
    throw std::runtime_error("Cannot Multiply matrix");
  }
  Matrix<std::complex<float>> ret(m_COL, mat.m_ROW);
  std::complex<float> alpha = 1;
  std::complex<float> beta = 1;
  cblas_cgemm(                        //
      CBLAS_ORDER::CblasColMajor,     //
      CBLAS_TRANSPOSE::CblasNoTrans,  //
      CBLAS_TRANSPOSE::CblasNoTrans,  //
      static_cast<int>(m_COL),        //
      static_cast<int>(mat.m_ROW),    //
      static_cast<int>(m_ROW),        //
      &alpha,                         //
      *this,                          //
      static_cast<int>(m_COL),        //
      mat,                            //
      static_cast<int>(mat.m_COL),    //
      &beta,                          //
      ret,                            //
      static_cast<int>(m_COL)         //
  );
  return ret;
}

template <FloatingPointType T>
Matrix<T>::Matrix(const Matrix<T> &mat, size_t col, size_t row)
    : m_COL(col), m_ROW(row) {
  if (mat.m_ROW * mat.m_COL != col * row) {
    throw std::runtime_error("配列の大きさが違います");
  }
  m_data = mat.m_data;
}

template <FloatingPointType T>
Matrix<T>::Matrix(Matrix<T> &&mat, size_t col, size_t row)
    : m_COL(col), m_ROW(row) {
  if (mat.m_ROW * mat.m_COL != col * row) {
    throw std::runtime_error("配列の大きさが違います");
  }
  m_data = std::move(mat.m_data);
}

// template <FloatingPointType T>
// Matrix<T>::Matrix(const Vector<T> &vec, int col, int row)
//     : m_COL(col), m_ROW(row) {
//   if (vec.m_SIZE != col * row) {
//     throw std::runtime_error("配列の大きさが違います");
//   }
//   m_data = vec.m_data;
// }

template <FloatingPointType T>
Matrix<T>::Matrix(std::vector<std::vector<T>> vec)
    : m_COL(vec.size()), m_ROW(vec[0].size()) {
  for (size_t i = 1; i < m_COL; ++i) {
    if (m_ROW != static_cast<size_t>(vec[i].size())) {
      throw std::runtime_error("Matrix Constructor recieve none matrix vector");
    }
  }
  // m_data.reserve(m_COL * m_ROW, 0.);
  m_data.resize(m_COL * m_ROW);
  for (size_t i = 0; i < m_ROW; ++i) {
    for (size_t j = 0; j < m_COL; ++j) {
      m_data[i * m_COL + j] = vec[j][i];
    }
  }
}

// template <FloatingPointType T>
// Matrix<T> Matrix<T>::Diag(Vector<T> vec, int col, int row) {
//   if (col == -1) {
//     col = vec.size();
//   }
//   if (row == -1) {
//     row = vec.size();
//   }
//   if (vec.size() > col && vec.size() > row) {
//     throw std::runtime_error(
//         "Not Correct args, col and row should be larger than vector size");
//   }
//   Matrix<T> ret(col, row);
//   for (int i = 0; i < vec.size(); ++i) {
//     ret[i][i] = vec[i];
//   }
//   return ret;
// }
//
template <FloatingPointType T>
void Matrix<T>::map(std::function<T(T)> fun) {
#pragma omp parallel for
  for (size_t i = 0; i < m_COL; i++) {
    for (size_t j = 0; j < m_ROW; j++) {
      (*this)[i][j] = fun((*this)[i][j]);
    }
  }
}

template <FloatingPointType T>
void Matrix<T>::map(std::function<T(T, size_t, size_t)> fun) {
#pragma omp parallel for
  for (size_t i = 0; i < m_COL; i++) {
    for (size_t j = 0; j < m_ROW; j++) {
      (*this)[i][j] = fun((*this)[i][j], i + 1, j + 1);
    }
  }
}

template <FloatingPointType T>
Matrix<T> Matrix<T>::Diag(std::initializer_list<T> vec,
                          std::optional<size_t> col,
                          std::optional<size_t> row) {
  if (!col.has_value()) {
    col = vec.size();
  }
  if (!row.has_value()) {
    row = vec.size();
  }
  if (vec.size() > col && vec.size() > row) {
    throw std::runtime_error(
        "Not Correct args, col and row should be larger than vector size");
  }
  Matrix<T> ret(col.value(), row.value());
  int counter{0};
  for (auto &&i : vec) {
    ret[counter][counter] = i;
    counter++;
  }
  return ret;
}

template <FloatingPointType T>
Matrix<T> Matrix<T>::Diag(T value, size_t size) {
  Matrix<T> ret(size, size);
  for (size_t i = 0; i < size; ++i) {
    ret[i][i] = value;
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
  for (size_t i = 1; i <= m_COL; ++i) {
    for (size_t j = 1; j <= m_ROW; ++j) {
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

template <FloatingPointType T>
T Matrix<T>::det() {
  T det(1.);
  Matrix m = *this;
  if (m.m_ROW != m.m_COL) {
    throw std::runtime_error("Matrix is not squre");
  }
  auto status = m.lu();
  for (size_t i = 0; i < m.m_COL; ++i) {
    det *= m[i][i];
    if (status.ipiv.at(i) != static_cast<int>(i + 1)) {
      det *= -1;
    }
  }
  return det;
}

template class Matrix<double>;
template class Matrix<float>;
template class Matrix<std::complex<double>>;
template class Matrix<std::complex<float>>;

}  // namespace Linalg
