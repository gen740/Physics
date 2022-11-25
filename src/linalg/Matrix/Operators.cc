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

template <FloatingPointType T>
Matrix<T> Matrix<T>::operator*(Matrix<T> mat) {
  if (m_ROW != mat.m_COL) {
    throw std::runtime_error("Cannot Multiply matrix with different size");
  }
  Matrix<T> ret(m_COL, mat.m_ROW);
  PHYSICS_POINTER_CONVERTIBLE_TYPE<T> alpha{1};
  PHYSICS_POINTER_CONVERTIBLE_TYPE<T> beta{1};
  PHYSICS_CONSTEXPR_BLAS_FUNC(T,                              //
                              gemm,                           //
                              CBLAS_ORDER::CblasColMajor,     //
                              CBLAS_TRANSPOSE::CblasNoTrans,  //
                              CBLAS_TRANSPOSE::CblasNoTrans,  //
                              static_cast<int>(m_COL),        //
                              static_cast<int>(mat.m_ROW),    //
                              static_cast<int>(m_ROW),        //
                              alpha,                          //
                              *this,                          //
                              static_cast<int>(m_COL),        //
                              mat,                            //
                              static_cast<int>(mat.m_COL),    //
                              beta,                           //
                              ret,                            //
                              static_cast<int>(m_COL)         //
  );
  return ret;
}

PHYSICS_REALIZE_MATRIX_MEMBER_FUNC;

}  // namespace Linalg
