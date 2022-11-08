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
int Matrix<double>::svd(Vector<double> &s, Matrix<double> &u,
                        Matrix<double> &v) {
  Vector<double> superb(std::min(m_ROW, m_COL));
  u.reshape(m_COL, m_COL);
  v.reshape(m_ROW, m_ROW);
  s.reshape(std::min(m_COL, m_ROW));
  return LAPACKE_dgesvd(LAPACK_COL_MAJOR, 'A', 'A', m_COL, m_ROW, *this, m_COL,
                        s, u, m_COL, v, m_ROW, superb);
}

template <>
int Matrix<float>::svd(Vector<float> &s, Matrix<float> &u, Matrix<float> &v) {
  Vector<float> superb(std::min(m_ROW, m_COL));
  u.reshape(m_COL, m_COL);
  v.reshape(m_ROW, m_ROW);
  s.reshape(std::min(m_COL, m_ROW));
  return LAPACKE_sgesvd(LAPACK_COL_MAJOR, 'A', 'A', m_COL, m_ROW, *this, m_COL,
                        s, u, m_COL, v, m_ROW, superb);
}

template <>
int Matrix<std::complex<double>>::svd(Vector<double> &s,
                                      Matrix<std::complex<double>> &u,
                                      Matrix<std::complex<double>> &v) {
  Vector<double> superb(std::min(m_ROW, m_COL));
  u.reshape(m_COL, m_COL);
  v.reshape(m_ROW, m_ROW);
  s.reshape(std::min(m_COL, m_ROW));
  return LAPACKE_zgesvd(LAPACK_COL_MAJOR, 'A', 'A', m_COL, m_ROW,
                        reinterpret_cast<_Complex double *>(&*this), m_COL, s,
                        reinterpret_cast<_Complex double *>(&u), m_COL,
                        reinterpret_cast<_Complex double *>(&v), m_ROW, superb);
}

template <>
int Matrix<std::complex<float>>::svd(Vector<float> &s,
                                     Matrix<std::complex<float>> &u,
                                     Matrix<std::complex<float>> &v) {
  Vector<float> superb(std::min(m_ROW, m_COL));
  u.reshape(m_COL, m_COL);
  v.reshape(m_ROW, m_ROW);
  s.reshape(std::min(m_COL, m_ROW));
  return LAPACKE_cgesvd(LAPACK_COL_MAJOR, 'A', 'A', m_COL, m_ROW,
                        reinterpret_cast<_Complex float *>(&*this), m_COL, s,
                        reinterpret_cast<_Complex float *>(&u), m_COL,
                        reinterpret_cast<_Complex float *>(&v), m_ROW, superb);
}

}  // namespace Linalg
