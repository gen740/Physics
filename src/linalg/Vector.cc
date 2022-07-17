#include "linalg/Vector.h"

#include <omp.h>

#include <array>
#include <exception>
#include <iomanip>
#include <iostream>
#include <vector>

namespace Linalg {

static int vector_precision = 4;

template <>
int Vector<double>::m_precision = vector_precision;

template <>
int Vector<float>::m_precision = vector_precision;

template <FloatingPointType T>
Vector<T>::Vector(const Matrix<T> &mat) {
  m_data = mat.m_data;
  m_SIZE = mat.m_ROW * mat.m_COL;
}

template <FloatingPointType T>
Vector<T>::Vector(Matrix<T> &&mat) {
  m_data = std::move(mat.m_data);
  m_SIZE = mat.m_ROW * mat.m_COL;
}

template <FloatingPointType T>
T &Vector<T>::operator()(int n) {
  if (1 <= n && n <= m_SIZE) {
    return m_data[n - 1];
  } else {
    throw std::runtime_error("Index out of range");
  }
}

template <FloatingPointType T>
T Vector<T>::operator()(int n) const {
  if (0 <= n && n < m_SIZE) {
    return m_data[n];
  } else {
    throw std::runtime_error("Index out of range");
  }
}

template <FloatingPointType T>
void Vector<T>::map(std::function<T(T)> func) {
  auto ret = Vector<T>(m_SIZE);
#pragma omp parallel for
  for (int i = 0; i < m_SIZE; ++i) {
    (*this)[i] = func((*this)[i]);
  }
}

template <FloatingPointType T>
void Vector<T>::map(std::function<T(T, T)> func) {
  auto ret = Vector<T>(m_SIZE);
#pragma omp parallel for
  for (int i = 0; i < m_SIZE; ++i) {
    (*this)[i] = func(i + 1, (*this)[i]);
  }
}

template <FloatingPointType T>
std::ostream &operator<<(std::ostream &os, const Vector<T> &vec) {
  os << "Vector n = " << vec.m_SIZE << "\n";
  os << std::setprecision(vec.m_precision) << std::scientific;
  for (int i = 0; i < vec.m_SIZE; ++i) {
    os << "\t" << vec(i);
  }
  os << std::defaultfloat;
  return os;
}

template std::ostream &operator<<(std::ostream &os, const Vector<double> &vec);
template std::ostream &operator<<(std::ostream &os, const Vector<float> &vec);

template class Vector<float>;
template class Vector<double>;

}  // namespace Linalg
