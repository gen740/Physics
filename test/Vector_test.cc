#include <gtest/gtest.h>
#include <linalg/Matrix.h>

#include <iostream>

using Linalg::Matrix;
using Linalg::Vector;

TEST(Vector, Basic) {
  Linalg::Vector<> m(4);
  EXPECT_EQ(m.size(), 4);
}

TEST(Vector, Map) {
  Linalg::Vector<> m1(5);
  for (int i = 1; i <= 5; i++) {
    m1(i) = i;
  }
  EXPECT_DOUBLE_EQ(m1(1), 1);
  EXPECT_DOUBLE_EQ(m1(3), 3);
  EXPECT_DOUBLE_EQ(m1(5), 5);

  m1.map([](double a) { return a * a; });
  EXPECT_DOUBLE_EQ(m1(1), 1);
  EXPECT_DOUBLE_EQ(m1(3), 9);
  EXPECT_DOUBLE_EQ(m1(5), 25);

  m1.map([](double i, double x) { return i * (x - 1); });
  EXPECT_DOUBLE_EQ(m1(1), 0);
  EXPECT_DOUBLE_EQ(m1(3), 24);
  EXPECT_DOUBLE_EQ(m1(5), 120);
}
