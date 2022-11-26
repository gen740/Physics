#include <gtest/gtest.h>

#include <generator/generator.hpp>

Generator::Generator g1() {
  for (int i = 0; i < 10; ++i) {
    std::cout << "From generator" << i << std::endl;
    co_yield true;
  }
}

TEST(Generator, Basic) {
  auto gen = g1();
  for (int i = 0; i < 20; ++i) {
    std::cout << i << " Times" << std::endl;
    gen.next();
  }
}
