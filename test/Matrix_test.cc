#include <gtest/gtest.h>
#include <linalg/Matrix.h>

#include <complex>

using Linalg::Matrix;
using Linalg::Vector;

template <typename T>
class MatrixTest : public ::testing::Test {
 public:
  static auto print_typename() -> void {
    std::cout << typeid(T).name() << std::endl;
  }
};

using FloatingTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(MatrixTest, FloatingTypes);

TYPED_TEST(MatrixTest, basic) {
  Matrix<TypeParam> m(123, 456);
  EXPECT_EQ(m.shape()[0], 123);
  EXPECT_EQ(m.shape()[1], 456);
  Matrix<std::complex<TypeParam>> mc(123, 456);
  EXPECT_EQ(mc.shape()[0], 123);
  EXPECT_EQ(mc.shape()[1], 456);
}

TYPED_TEST(MatrixTest, IndexAccess) {
  Matrix<TypeParam> m(3, 3);
  m(1, 1) = 1.242;
  EXPECT_FLOAT_EQ(m[0][0], 1.242);
  EXPECT_FLOAT_EQ(m(1, 1), 1.242);
  m(3, 3) = 3;
  EXPECT_FLOAT_EQ(m(3, 3), 3);
  m[1][1] = 4;
  EXPECT_FLOAT_EQ(m(2, 2), 4);
  Matrix<std::complex<TypeParam>> mc(3, 3);
  mc(1, 1) = 1.242;
  EXPECT_FLOAT_EQ(mc[0][0].real(), 1.242);
  EXPECT_FLOAT_EQ(mc(1, 1).real(), 1.242);
  mc(3, 3) = 3;
  EXPECT_FLOAT_EQ(mc(3, 3).real(), 3);
  mc[1][1] = 4;
  EXPECT_FLOAT_EQ(mc(2, 2).real(), 4);
}

// cout/save でどれだけの精度で出力するかが設定できる。
TEST(Matrix, ChangePrecision) {
  auto check_prec = [](int n) {
    EXPECT_EQ(Matrix<double>::get_precision(), n);
    EXPECT_EQ(Matrix<float>::get_precision(), n);
    EXPECT_EQ(Matrix<std::complex<double>>::get_precision(), n);
    EXPECT_EQ(Matrix<std::complex<float>>::get_precision(), n);
  };
  Linalg::Matrix<> m(2, 2);
  // デフォルトの精度は 4桁
  check_prec(4);
  // メンバーを介して精度を変えられる。
  Linalg::Matrix<double>::set_precision(5);
  // 精度を変えた後は double / float の精度が変わる
  EXPECT_EQ(m.get_precision(), 5);
  check_prec(5);
  Linalg::Matrix<std::complex<double>>::set_precision(8);
  check_prec(8);
  Linalg::Matrix<std::complex<float>>::set_precision(12);
  check_prec(12);
}

TYPED_TEST(MatrixTest, Advance) {
  Linalg::Matrix<TypeParam> m(5, 5);
  EXPECT_EQ(m.to_vec().size(), 25);
  m(3, 3) = 5;
  EXPECT_FLOAT_EQ(m.to_vec()(13), 5);
  for (int i = 1; i <= 5; ++i) {
    m(i, i) = 1;
  }
  auto state = m.lu();
  EXPECT_EQ(state.status, 0);
  std::cout << m.det() << std::endl;
  EXPECT_FLOAT_EQ(m.det(), 1);

  // Diag Matrix 作成の Test
  Vector<double> S({3., 1., 4., 1., 5.});
  // 引数に Vector を渡してやればそれを対角成分に持つ対角行列を作ることができる
  auto diag_mat = Matrix<double>::Diag(Vector<double>(S));
  for (int i = 0; i < diag_mat.col_size(); ++i) {
    for (int j = 0; j < diag_mat.row_size(); ++j) {
      if (i == j) {
        EXPECT_FLOAT_EQ(diag_mat[i][j], S[i]);
      } else {
        EXPECT_FLOAT_EQ(diag_mat[i][j], 0);
      }
    }
  }
}

TEST(Matrix, Operation) {
  Matrix<double> m({
      {1, 2, 3},  //
      {4, 5, 6}   //
  });
  Matrix<double> l({{1, 2}, {3, 4}, {5, 6}});
  std::cout << m << std::endl;
  std::cout << l << std::endl;
  std::cout << m * l << std::endl;
}

TEST(Matrix, SVD) {
  Matrix<double> m({
      {1, 2, 3, 4},    //
      {5, 6, 7, 8},    //
      {9, 10, 11, 12}  //
  });
  std::cout << m << std::endl;
  Vector<double> s;
  Matrix<double> u;
  Matrix<double> v;
  m.svd(s, u, v);
  std::cout << "S = " << s << std::endl;
  std::cout << "U = " << u << std::endl;
  std::cout << "Vt = " << v << std::endl;

  Matrix<double> s2(3, 4);
  for (int i = 0; i < 3; ++i) {
    s2[i][i] = s[i];
  }

  auto m3 = u * s2 * v;
  // m3.reshape(3, 4);
  std::cout << m3 << std::endl;
}
