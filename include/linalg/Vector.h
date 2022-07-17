#pragma once

#include <array>
#include <functional>
#include <iosfwd>
#include <vector>

#include "linalg/Matrix.h"
#include "linalg/Types.h"

namespace Linalg {

template <FloatingPointType T>
class Matrix;

template <FloatingPointType T = double>
class Vector {
 public:
  Vector(int size) : m_data(size), m_SIZE(size) {}
  Vector() : m_data(0), m_SIZE(0) {}
  Vector(const Vector<T> &vec) : m_data(vec.m_data), m_SIZE(vec.m_SIZE) {}
  Vector(const Matrix<T> &mat);
  Vector(Matrix<T> &&mat);
  Vector(std::vector<T> vec) : m_data(vec), m_SIZE(vec.size()) {}

  int size() { return m_SIZE; };

  void reshape(int size) {
    m_SIZE = size;
    m_data.resize(size);
  }

  // 1-index
  T &operator()(int n);

  // 1-index
  T operator()(int n) const;

  operator T *() { return m_data.data(); }

  void map(std::function<T(T)> func);
  void map(std::function<T(T, T)> func);

  Matrix<T> to_mat(int col, int row) { return Matrix<T>(*this, col, row); }

  // 内部のvector表現を得る
  std::vector<T> data() { return m_data; }

  template <FloatingPointType U>
  friend std::ostream &operator<<(std::ostream &os, const Vector<U> &vec);

  friend Matrix<T>;

 private:
  std::vector<T> m_data;
  int m_SIZE;
  static int m_precision;
};

}  // namespace Linalg
