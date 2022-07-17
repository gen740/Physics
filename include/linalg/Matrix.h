#pragma once
#include <lapacke.h>

#include <array>
#include <iosfwd>
#include <vector>

#include "linalg/Types.h"
#include "linalg/Vector.h"

namespace Linalg {

template <FloatingPointType T>
class Vector;

struct LU_status {
  int status;
  std::vector<int> ipiv;
  operator int() const { return status; }
  LU_status(int size) : ipiv(size) {}
};

template <FloatingPointType T = double>
class Matrix {
 public:
  // Constructors
  Matrix(int col, int row) : m_data(col * row), m_COL(col), m_ROW(row) {}
  Matrix() : m_data(0), m_COL(0), m_ROW(0) {}
  Matrix(const Matrix &mat)
      : m_data(mat.m_data), m_COL(mat.m_COL), m_ROW(mat.m_ROW) {}
  Matrix(const Matrix<T> &mat, int col, int row);
  Matrix(Matrix<T> &&mat, int col, int row);
  Matrix(const Vector<T> &vec, int col, int row);
  Matrix(std::vector<std::vector<T>> vec);

  void reshape(int col, int row) {
    if (col == m_COL && row == m_ROW) {
      return;
    }
    m_COL = col;
    m_ROW = row;
    m_data.resize(col * row, 0.);
  }

  static Matrix<T> Diag(Vector<T> vec, int col = -1, int row = -1);

 public:
  // {col, row}
  std::array<int, 2> shape() const { return {m_COL, m_ROW}; }
  int col_size() const { return m_COL; };
  int row_size() const { return m_ROW; };

  // destructive LU
  LU_status lu();

  // destructive svd
  int svd(Vector<T> &s, Matrix<T> &u, Matrix<T> &v);

  // destructive det
  T det();

  friend Vector<T>;

  // Vector に変換する
  Vector<T> to_vec() { return Vector<T>(*this); }

  // 内部のvector表現を得る
  std::vector<T> data() { return m_data; }

  template <FloatingPointType U>
  friend std::ostream &operator<<(std::ostream &os, const Matrix<U> &mat);

  T &operator()(int col, int row);
  T operator()(int col, int row) const;
  T *operator[](int i) { return &m_data.data()[i * m_COL]; }

  operator T *() { return m_data.data(); }

  Matrix<T> operator*(Matrix<T> mat);

  void save(const char *filename, const char delimeter = ',',
            bool is_scientific = true);

  static void set_precision(int precision);
  static int get_precision();

 private:
  static int *const m_precision;

  std::vector<T> m_data;
  int m_COL{0}, m_ROW{0};
};

}  // namespace Linalg
