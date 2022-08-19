#include <fraction/Fraction.h>
#include <gtest/gtest.h>

using Fraction::frac;

TEST(Fraction, Constructor) {
  {
    frac<> f(3, 4);
    std::cout << f << std::endl;
    EXPECT_DOUBLE_EQ(f.eval(), 3. / 4.);
  }
  {
    frac<> f;
    std::cout << f << std::endl;
    EXPECT_DOUBLE_EQ(f.eval(), 1);
  }
  {
    frac<> f(31);
    std::cout << f << std::endl;
    EXPECT_DOUBLE_EQ(f.eval(), 31);
  }
  frac<> f(4, 5);
  frac<> g(f);
  EXPECT_DOUBLE_EQ(g.eval(), 4. / 5.);
  f = frac<>(3, 7);
  EXPECT_DOUBLE_EQ(f.eval(), 3. / 7.);
}

TEST(Fraction, Basic) {
  frac<> f1(3, 4);
  frac<> f2(6, 7);
  EXPECT_DOUBLE_EQ((f1 + f2).eval(), 45. / 28);
  EXPECT_DOUBLE_EQ((f1 - f2).eval(), -3. / 28);
  EXPECT_DOUBLE_EQ((-f2).eval(), -6. / 7);
  EXPECT_DOUBLE_EQ((f1 * f2).eval(), 9. / 14);
  EXPECT_DOUBLE_EQ((f1 / f2).eval(), 7. / 8);

  std::cout << (f1 > f2) << std::endl;
  std::cout << (f1 == f2) << std::endl;
}

TEST(Fraction, Advance) {}
