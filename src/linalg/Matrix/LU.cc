#include "Utilities.h"
#include "linalg/Matrix.h"
#include "linalg/Types.h"

#ifdef PHYSICS_USE_MKL
#include <mkl_cblas.h>
#include <mkl_lapacke.h>
#else
#include <cblas.h>
#include <lapacke.h>
#endif

namespace Linalg {

template <FloatingPointType T>
LU_status Matrix<T>::lu() {
  LU_status status(static_cast<int>(m_ROW));
  PHYSICS_CONSTEXPR_LAPACKE_FUNC_WITH_RETURN(  //
      T,                                       //
      getrf,                                   //
      status.status,                           //
      LAPACK_COL_MAJOR,                        //
      static_cast<int>(m_COL),                 //
      static_cast<int>(m_ROW),                 //
      (*this),                                 //
      static_cast<int>(m_COL),                 //
      status.ipiv.data()                       //
  );
  return status;
}

template <FloatingPointType T>
T Matrix<T>::det() {
  T det(1.);
  Matrix m = *this;
  if (m.m_ROW != m.m_COL) [[unlikely]] {
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

PHYSICS_REALIZE_MATRIX_MEMBER_FUNC;

}  // namespace Linalg
