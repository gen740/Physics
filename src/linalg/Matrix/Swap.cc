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

namespace Linalg {

template <FloatingPointType T>
void Matrix<T>::swap(size_t i, size_t j, bool COL) {
  if (COL) {
    if (i > m_COL || j > m_COL) {
      throw std::runtime_error("arg is larger than COL size");
    }
  } else {
    if (i > m_ROW || j > m_ROW) {
      throw std::runtime_error("arg is larger than ROW size");
    }
  }
  if (i == j) {
    return;
  }
  if (COL) {
    PHYSICS_CONSTEXPR_BLAS_FUNC(      //
        T,                            //
        swap,                         //
        static_cast<blasint>(m_COL),  //
        &(*this)[0][i - 1],           //
        static_cast<blasint>(m_COL),  //
        &(*this)[0][j - 1],           //
        static_cast<blasint>(m_COL)   //
    );
  } else {
    PHYSICS_CONSTEXPR_BLAS_FUNC(      //
        T,                            //
        swap,                         //
        static_cast<blasint>(m_ROW),  //
        (*this)[i - 1],               //
        1,                            //
        (*this)[j - 1],               //
        1                             //
    );
  }
}

PHYSICS_REALIZE_MATRIX_MEMBER_FUNC;

}  // namespace Linalg
