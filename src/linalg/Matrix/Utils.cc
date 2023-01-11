#include <optional>

#include "Utilities.h"
#include "linalg/Matrix.h"

#ifdef PHYSICS_USE_MKL
#include <mkl_cblas.h>
#include <mkl_lapacke.h>
#else
#include <cblas.h>
#include <lapacke.h>
#endif

#include <exception>
#include <fstream>
#include <initializer_list>

namespace Linalg {

template <FloatingPointType T>
Matrix<T>::Matrix(size_t col, size_t row, T val)
    : m_data(col * row, val), m_COL(col), m_ROW(row) {}

template <FloatingPointType T>
Matrix<T>::Matrix() : m_data(0) {}

template <FloatingPointType T>
Matrix<T>::Matrix(const Matrix &mat)
    : m_data(mat.m_data), m_COL(mat.m_COL), m_ROW(mat.m_ROW) {}

template <FloatingPointType T>
Matrix<T>::Matrix(const Matrix<T> &mat, size_t col, size_t row)
    : m_COL(col), m_ROW(row) {
  if (mat.m_ROW * mat.m_COL != col * row) [[unlikely]] {
    throw std::runtime_error("配列の大きさが違います");
  }
  m_data = mat.m_data;
}

template <FloatingPointType T>
Matrix<T>::Matrix(Matrix<T> &&mat, size_t col, size_t row)
    : m_COL(col), m_ROW(row) {
  if (mat.m_ROW * mat.m_COL != col * row) [[unlikely]] {
    throw std::runtime_error("配列の大きさが違います");
  }
  m_data = std::move(mat.m_data);
}

template <FloatingPointType T>
Matrix<T>::Matrix(std::vector<std::vector<T>> vec)
    : m_COL(vec.size()), m_ROW(vec[0].size()) {
  for (size_t i = 1; i < m_COL; ++i) {
    if (m_ROW != static_cast<size_t>(vec[i].size())) [[unlikely]] {
      throw std::runtime_error("Matrix Constructor recieve none matrix vector");
    }
  }
  m_data.resize(m_COL * m_ROW);
  for (size_t i = 0; i < m_ROW; ++i) {
    for (size_t j = 0; j < m_COL; ++j) {
      m_data[i * m_COL + j] = vec[j][i];
    }
  }
}

template <FloatingPointType T>
void Matrix<T>::reshape(size_t col, size_t row) {
  if (col == m_COL && row == m_ROW) {
    return;
  }
  m_COL = col;
  m_ROW = row;
  m_data.resize(col * row, 0.);
}

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
  if (vec.size() > col && vec.size() > row) [[unlikely]] {
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

PHYSICS_REALIZE_MATRIX_MEMBER_FUNC;

}  // namespace Linalg
