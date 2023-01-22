#pragma once

#include <array>
#include <complex>
#include <functional>
#include <initializer_list>
#include <iosfwd>
#include <memory>
#include <optional>
#include <vector>

#include "linalg/Types.h"
#include "linalg/UtilityClasses.h"

namespace Linalg {

/*! Matrix Clasas (COL MAJOR)
 *
 * 1 2 3
 * 4 5 6
 * 7 8 9
 *
 * Matrix access (1-index):
 *
 * m.(col, row) or m[row - 1][col - 1]
 *
 * */
template <FloatingPointType T = double>
class Matrix {
 public:
  // Constructors
  using BaseType = base_type_t<T>;
  explicit Matrix(size_t col, size_t row, T val = 0.0);
  Matrix();
  Matrix(Matrix &&) noexcept = default;
  Matrix &operator=(const Matrix &) = default;
  Matrix &operator=(Matrix &&) noexcept = default;
  Matrix(const Matrix &mat);
  Matrix(const Matrix<T> &mat, size_t col, size_t row);
  Matrix(Matrix<T> &&mat, size_t col, size_t row);
  Matrix(std::vector<std::vector<T>> vec);

  ~Matrix() = default;

  void reshape(size_t col, size_t row);

  void map(std::function<T(T)> fun);
  void map(std::function<T(T, size_t, size_t)> fun);

  // static Matrix<T> Diag(std::vector<T> vec, size_t col = -1, size_t row =
  // -1);
  static Matrix<T> Diag(std::initializer_list<T> vec,
                        std::optional<size_t> col = std::nullopt,
                        std::optional<size_t> row = std::nullopt);
  static Matrix<T> Diag(T value, size_t size);
  // static Matrix<T> Diag(T value, size_t col, size_t row);

  // {col, row}
  [[nodiscard]] std::array<size_t, 2> shape() const { return {m_COL, m_ROW}; }
  [[nodiscard]] size_t col_size() const { return m_COL; };
  [[nodiscard]] size_t row_size() const { return m_ROW; };

  // destructive LU
  LU_status lu();

  // destructive svd
  int svd(Matrix<BaseType> &s, Matrix<T> &u, Matrix<T> &v);

  // destructive det
  T det();

  // swap i and j
  void swap(size_t i, size_t j, bool COL = true) noexcept(false);

  // 内部のvector表現を得る
  std::vector<T> data() { return m_data; }

  template <FloatingPointType U>
  friend std::ostream &operator<<(std::ostream &os, const Matrix<U> &mat);

  T &operator()(size_t col, size_t row);
  T operator()(size_t col, size_t row) const;
  T *operator[](size_t i) { return &m_data.data()[i * m_COL]; }

  // T *operator[](size_t i) const { return &m_data.data()[i * m_COL]; }

  operator T *();
  operator T const *();
  operator void *();
  operator void const *();

  template <class U>
  operator U *();

  Matrix<T> operator*(Matrix<T> mat);

  void save(const char *filename, char delimeter = ',',
            bool is_scientific = true);

  static void set_precision(int precision);
  static int get_precision();

 private:
  static const std::shared_ptr<int> m_precision;

  std::vector<T> m_data;
  size_t m_COL{0}, m_ROW{0};
};

using DMatrix = Matrix<double>;
using FMatrix = Matrix<float>;
using ZMatrix = Matrix<std::complex<double>>;
using CMatrix = Matrix<std::complex<float>>;

}  // namespace Linalg
