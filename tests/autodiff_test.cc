#include <gtest/gtest.h>

#include <autodiff/autodiff.hpp>
#include <iostream>

using autodiff::dual;

template <class T>
void func1(T a) {
  std::cout << a << std::endl;
}

// template <template <class> class T>
// void call(std::function<void(T<int>)> fun) {
//   fun(3);
// }

TEST(autodiff, multiple) {
  auto quad_func = []<autodiff::Dual T>(T a) -> T { return a * a; };

  auto [d0, d1, d2, d3] = quad_func(dual<3>(3)).value();

  EXPECT_EQ(d0, 9);
  EXPECT_EQ(d1, 6);
  EXPECT_EQ(d2, 2);
  EXPECT_EQ(d3, 0);

  // std::cout << quad_func(dual<3>(3)).value()[3] << std::endl;
  // std::cout << func<int>(3) << std::endl;
  // std::cout << autodiff::derivative<1, double>(func, 3) << std::endl;
  // std::cout << autodiff::derivative(func, 3) << std::endl;
  // autodiff::dual<double>::
}
