#pragma once

namespace Linalg {

template <class T>
struct PHYSICS_CONVERSION_TYPE {
  T data;
  operator T() { return data; }
  operator T*() { return &data; }
};

}  // namespace Linalg

#define PHYSICS_CONSTEXPR_BLAS_FUNC(TypeParam, blas_func, ...)            \
  if constexpr (std::is_same_v<double, TypeParam>) {                      \
    cblas_d##blas_func(__VA_ARGS__);                                      \
  } else if constexpr (std::is_same_v<float, TypeParam>) {                \
    cblas_s##blas_func(__VA_ARGS__);                                      \
  } else if constexpr (std::is_same_v<std::complex<double>, TypeParam>) { \
    cblas_z##blas_func(__VA_ARGS__);                                      \
  } else if constexpr (std::is_same_v<std::complex<float>, TypeParam>) {  \
    cblas_c##blas_func(__VA_ARGS__);                                      \
  }

#define PHYSICS_CONSTEXPR_LAPACKE_FUNC(TypeParam, lapacke_func, ...)      \
  if constexpr (std::is_same_v<double, TypeParam>) {                      \
    LAPACKE_d##lapacke_func(__VA_ARGS__);                                 \
  } else if constexpr (std::is_same_v<float, TypeParam>) {                \
    LAPACKE_s##lapacke_func(__VA_ARGS__);                                 \
  } else if constexpr (std::is_same_v<std::complex<double>, TypeParam>) { \
    LAPACKE_z##lapacke_func(__VA_ARGS__);                                 \
  } else if constexpr (std::is_same_v<std::complex<float>, TypeParam>) {  \
    LAPACKE_c##lapacke_func(__VA_ARGS__);                                 \
  }

#define PHYSICS_CONSTEXPR_LAPACKE_FUNC_WITH_RETURN(TypeParam, lapacke_func, \
                                                   return_val, ...)         \
  if constexpr (std::is_same_v<double, TypeParam>) {                        \
    return_val = LAPACKE_d##lapacke_func(__VA_ARGS__);                      \
  } else if constexpr (std::is_same_v<float, TypeParam>) {                  \
    return_val = LAPACKE_s##lapacke_func(__VA_ARGS__);                      \
  } else if constexpr (std::is_same_v<std::complex<double>, TypeParam>) {   \
    return_val = LAPACKE_z##lapacke_func(__VA_ARGS__);                      \
  } else if constexpr (std::is_same_v<std::complex<float>, TypeParam>) {    \
    return_val = LAPACKE_c##lapacke_func(__VA_ARGS__);                      \
  }

#define PHYSICS_REALIZE_MATRIX_MEMBER_FUNC     \
  template class Matrix<double>;               \
  template class Matrix<float>;                \
  template class Matrix<std::complex<double>>; \
  template class Matrix<std::complex<float>>;
