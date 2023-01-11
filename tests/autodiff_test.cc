#include <autodiff/autodiff.h>
#include <gtest/gtest.h>

#include <iostream>

using autodiff::dual;

template <typename T>
class AutodiffTest : public ::testing::Test {
 public:
  static auto print_typename() -> void {
    std::cout << typeid(T).name() << std::endl;
  }
};

using FloatingTypes = ::testing::Types<double, float>;

TYPED_TEST_SUITE(AutodiffTest, FloatingTypes);

TYPED_TEST(AutodiffTest, multiple) {
  auto quad_func1 = []<autodiff::Dual T>(T a) -> T { return 3.1 * a * a; };
  auto quad_func2 = []<autodiff::Dual T>(T a) -> T {
    return a * a * a * 2.7 * a * a;
  };

  auto multivariate_multiple1 = []<autodiff::Dual T>(T a, T b) -> T {
    return a * b;
  };
  auto multivariate_multiple2 = []<autodiff::Dual T>(T a, T b) -> T {
    return a * a * b * b * b;
  };

  dual<5, TypeParam> x = 2.4;
  {
    auto result = quad_func1(x);
    EXPECT_FLOAT_EQ(result.derivative({{x, 0}}), 3.1 * 2.4 * 2.4);
    EXPECT_FLOAT_EQ(result.derivative({{x, 1}}), 3.1 * 2 * 2.4);
    EXPECT_FLOAT_EQ(result.derivative({{x, 2}}), 3.1 * 2);
    EXPECT_FLOAT_EQ(result.derivative({{x, 3}}), 0);
    EXPECT_FLOAT_EQ(result.derivative({{x, 4}}), 0);
    EXPECT_FLOAT_EQ(result.derivative({{x, 5}}), 0);
  }
  {
    auto result = quad_func2(x);
    EXPECT_FLOAT_EQ(result.derivative({{x, 0}}), 2.7 * std::pow(2.4, 5));
    EXPECT_FLOAT_EQ(result.derivative({{x, 1}}), 2.7 * 5 * std::pow(2.4, 4));
    EXPECT_FLOAT_EQ(result.derivative({{x, 2}}),
                    2.7 * 5 * 4 * std::pow(2.4, 3));
    EXPECT_FLOAT_EQ(result.derivative({{x, 3}}),
                    2.7 * 5 * 4 * 3 * std::pow(2.4, 2));
    EXPECT_FLOAT_EQ(result.derivative({{x, 4}}), 2.7 * 5 * 4 * 3 * 2 * 2.4);
    EXPECT_FLOAT_EQ(result.derivative({{x, 5}}), 2.7 * 5 * 4 * 3 * 2);
    EXPECT_FLOAT_EQ(result.derivative({{x, 6}}), 0);
  }

  dual<5, TypeParam> y = 3.7;
  {
    auto result = multivariate_multiple1(x, y);
    EXPECT_FLOAT_EQ(result.derivative({{x, 0}, {y, 0}}), 2.4 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 1}, {y, 0}}), 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 0}, {y, 1}}), 2.4);
    EXPECT_FLOAT_EQ(result.derivative({{x, 1}, {y, 1}}), 1);
    EXPECT_FLOAT_EQ(result.derivative({{x, 2}, {y, 1}}), 0);
  }
  {
    auto result = multivariate_multiple2(x, y);
    EXPECT_FLOAT_EQ(result.derivative({{x, 0}, {y, 0}}),
                    2.4 * 2.4 * 3.7 * 3.7 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 1}, {y, 0}}),
                    2. * 2.4 * 3.7 * 3.7 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 2}, {y, 0}}), 2. * 3.7 * 3.7 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 3}, {y, 0}}), 0.);
    EXPECT_FLOAT_EQ(result.derivative({{x, 0}, {y, 1}}),
                    3 * 2.4 * 2.4 * 3.7 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 1}, {y, 1}}), 6 * 2.4 * 3.7 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 2}, {y, 1}}), 6 * 3.7 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 0}, {y, 2}}), 6 * 2.4 * 2.4 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 1}, {y, 2}}), 12 * 2.4 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 2}, {y, 2}}), 12 * 3.7);
    EXPECT_FLOAT_EQ(result.derivative({{x, 0}, {y, 3}}), 6 * 2.4 * 2.4);
    EXPECT_FLOAT_EQ(result.derivative({{x, 1}, {y, 3}}), 12 * 2.4);
    EXPECT_FLOAT_EQ(result.derivative({{x, 2}, {y, 3}}), 12);
    EXPECT_FLOAT_EQ(result.derivative({{x, 3}, {y, 3}}), 0);
  }
}

TYPED_TEST(AutodiffTest, div) {
  auto quad_func1 = []<autodiff::Dual T>(T a) -> T { return 1. / a; };

  dual<5, TypeParam> x = 3;
  {
    auto result = quad_func1(x);
    std::cout << result.derivative({{x, 0}}) << std::endl;
    std::cout << result.derivative({{x, 1}}) << std::endl;
    std::cout << result.derivative({{x, 2}}) << std::endl;
    std::cout << result.derivative({{x, 3}}) << std::endl;
  }
}

// template <class T>
// void func1(T a) {
//   std::cout << a << std::endl;
// }

// template <template <class> class T>
// void call(std::function<void(T<int>)> fun) {
//   fun(3);
// }
