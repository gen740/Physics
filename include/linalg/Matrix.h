#pragma once

#include <array>
#include <initializer_list>
#include <iosfwd>
#include <type_traits>
#include <vector>

#include "linalg/Types.h"

namespace Linalg {

struct LU_status {
  int status{};
  std::vector<int> ipiv;
  explicit operator int() const { return status; }
  explicit LU_status(int size) : ipiv(size) {}
};

template <FloatingPointType T = double>
class Matrix {
 public:
  // Constructors
  using BaseType = std::conditional_t<
      std::is_same_v<T, double>, double,
      std::conditional_t<
          std::is_same_v<T, float>, float,
          std::conditional_t<std::is_same_v<T, std::complex<double>>, double,
                             float>>>;
  explicit Matrix(int col, int row, T val = 0.)
      : m_data(col * row, val), m_COL(col), m_ROW(row) {}
  Matrix() : m_data(0) {}
  Matrix(Matrix &&) noexcept = default;
  Matrix &operator=(const Matrix &) = default;
  Matrix &operator=(Matrix &&) noexcept = default;
  Matrix(const Matrix &mat)
      : m_data(mat.m_data), m_COL(mat.m_COL), m_ROW(mat.m_ROW) {}
  Matrix(const Matrix<T> &mat, int col, int row);
  Matrix(Matrix<T> &&mat, int col, int row);
  Matrix(std::vector<std::vector<T>> vec);
  ~Matrix() = default;

  void reshape(int col, int row) {
    if (col == m_COL && row == m_ROW) {
      return;
    }
    m_COL = col;
    m_ROW = row;
    m_data.resize(col * row, 0.);
  }

  // static Matrix<T> Diag(std::vector<T> vec, int col = -1, int row = -1);
  static Matrix<T> Diag(std::initializer_list<T> vec, int col = -1,
                        int row = -1);
  static Matrix<T> Diag(T value, int size);
  // static Matrix<T> Diag(T value, int col, int row);

  // {col, row}
  [[nodiscard]] std::array<int, 2> shape() const { return {m_COL, m_ROW}; }
  [[nodiscard]] int col_size() const { return m_COL; };
  [[nodiscard]] int row_size() const { return m_ROW; };

  // destructive LU
  LU_status lu();

  // destructive svd
  int svd(Matrix<BaseType> &s, Matrix<T> &u, Matrix<T> &v);

  // destructive det
  T det();

  // 内部のvector表現を得る
  std::vector<T> data() { return m_data; }

  template <FloatingPointType U>
  friend std::ostream &operator<<(std::ostream &os, const Matrix<U> &mat);

  T &operator()(int col, int row);
  T operator()(int col, int row) const;
  T *operator[](int i) { return &m_data.data()[i * m_COL]; }

  operator T *() { return m_data.data(); }

  Matrix<T> operator*(Matrix<T> mat);
  // Matrix<T> operator*(Vector<T> mat);

  void save(const char *filename, char delimeter = ',',
            bool is_scientific = true);

  static void set_precision(int precision);
  static int get_precision();

 private:
  static int *const m_precision;

  std::vector<T> m_data;
  int m_COL{0}, m_ROW{0};
};

using DMatrix = Matrix<double>;
using FMatrix = Matrix<float>;
using ZMatrix = Matrix<std::complex<double>>;
using CMatrix = Matrix<std::complex<float>>;

}  // namespace Linalg
