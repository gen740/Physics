#include "linalg/Matrix.h"

#ifdef PHYSICS_USE_MKL
#include <mkl_cblas.h>
#include <mkl_lapacke.h>
#else
#include <cblas.h>
#include <lapacke.h>
#endif

namespace Linalg {
template <>
LU_status Matrix<double>::lu() {
  LU_status status(m_ROW);
  status.status = LAPACKE_dgetrf(LAPACK_COL_MAJOR, m_COL, m_ROW, m_data.data(),
                                 m_COL, status.ipiv.data());
  return status;
}

template <>
LU_status Matrix<float>::lu() {
  LU_status status(m_ROW);
  status.status = LAPACKE_sgetrf(LAPACK_COL_MAJOR, m_COL, m_ROW, m_data.data(),
                                 m_COL, status.ipiv.data());
  return status;
}

template <>
LU_status Matrix<std::complex<double>>::lu() {
  LU_status status(m_ROW);
  status.status =
      LAPACKE_zgetrf(LAPACK_COL_MAJOR, m_COL, m_ROW,
                     reinterpret_cast<_Complex double *>(m_data.data()), m_COL,
                     status.ipiv.data());
  return status;
}

template <>
LU_status Matrix<std::complex<float>>::lu() {
  LU_status status(m_ROW);
  status.status =
      LAPACKE_cgetrf(LAPACK_COL_MAJOR, m_COL, m_ROW,
                     reinterpret_cast<_Complex float *>(m_data.data()), m_COL,
                     status.ipiv.data());
  return status;
}
}  // namespace Linalg
