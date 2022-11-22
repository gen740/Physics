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
int Matrix<T>::svd(Matrix<BaseType> &s, Matrix<T> &u, Matrix<T> &v) {
  Matrix<BaseType> superb(std::min(m_ROW, m_COL), 1);
  u.reshape(m_COL, m_COL);
  v.reshape(m_ROW, m_ROW);
  s.reshape(std::min(m_COL, m_ROW), 1);
  int ret;
  PHYSICS_CONSTEXPR_LAPACKE_FUNC_WITH_RETURN(  //
      T,                                       //
      gesvd,                                   //
      ret,                                     //
      LAPACK_COL_MAJOR,                        //
      'A',                                     //
      'A',                                     //
      static_cast<int>(m_COL),                 //
      static_cast<int>(m_ROW),                 //
      *this,                                   //
      static_cast<int>(m_COL),                 //
      s,                                       //
      u,                                       //
      static_cast<int>(m_COL),                 //
      v,                                       //
      static_cast<int>(m_ROW),                 //
      superb                                   //
  );
  return ret;
}

PHYSICS_REALIZE_MATRIX_MEMBER_FUNC;

}  // namespace Linalg
