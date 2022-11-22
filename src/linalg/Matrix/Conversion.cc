#include "Utilities.h"
#include "linalg/Matrix.h"

#ifdef PHYSICS_USE_MKL
#include <mkl_cblas.h>
#include <mkl_lapacke.h>
#else
#include <cblas.h>
#include <lapacke.h>
#endif

namespace Linalg {

template <FloatingPointType T>
Matrix<T>::operator T *() {
  return m_data.data();
}

template <FloatingPointType T>
Matrix<T>::operator T const *() {
  return m_data.data();
}

template <FloatingPointType T>
Matrix<T>::operator void *() {
  return m_data.data();
}

template <FloatingPointType T>
Matrix<T>::operator void const *() {
  return m_data.data();
}

template <>
template <>
Matrix<std::complex<double>>::operator _Complex double *() {
  return reinterpret_cast<_Complex double *>(m_data.data());
}

template <>
template <>
Matrix<std::complex<float>>::operator _Complex float *() {
  return reinterpret_cast<_Complex float *>(m_data.data());
}

PHYSICS_REALIZE_MATRIX_MEMBER_FUNC;

}  // namespace Linalg
