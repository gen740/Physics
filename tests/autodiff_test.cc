#include <gtest/gtest.h>

#include <autodiff/autodiff.hpp>
#include <iostream>

template <class T>
T func(T a) {
  return a * a;
}

TEST(autodiff, dummy) {
  std::cout << func<autodiff::dual<3>>(3).value()[2] << std::endl;
  // std::cout << func<int>(3) << std::endl;
  // std::cout << autodiff::derivative<1, double>(func, 3) << std::endl;
  // std::cout << autodiff::derivative(func, 3) << std::endl;
  // autodiff::dual<double>::
}
