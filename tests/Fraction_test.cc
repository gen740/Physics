#include <fraction/Fraction.h>
#include <gtest/gtest.h>

using Fraction::frac;

TEST(Fraction, Constructor) {
  {
    frac<> f(3, 4);
    EXPECT_DOUBLE_EQ(f.eval(), 3. / 4.);
  }
  {
    frac<> f;
    EXPECT_DOUBLE_EQ(f.eval(), 0);
  }
  {
    frac<> f(31);
    EXPECT_DOUBLE_EQ(f.eval(), 31);
  }
  frac<> f(4, 5);
  frac<> g(f);
  EXPECT_DOUBLE_EQ(g.eval(), 4. / 5.);
  f = frac<>(3, 7);
  EXPECT_DOUBLE_EQ(f.eval(), 3. / 7.);
}

TEST(Fraction, Basic) {
  frac<int> f1(3, 4);
  frac<int32_t> f2(6, 7);
  EXPECT_DOUBLE_EQ((f1 + f2).eval(), 45. / 28);

  EXPECT_DOUBLE_EQ((f1 + 3).eval(), 15. / 4);
  EXPECT_DOUBLE_EQ((3 + f1).eval(), 15. / 4);

  EXPECT_DOUBLE_EQ((f1 - 3).eval(), -9. / 4);
  EXPECT_DOUBLE_EQ((3 - f1).eval(), 9. / 4);

  EXPECT_DOUBLE_EQ((f1 * 3).eval(), 9. / 4);
  EXPECT_DOUBLE_EQ((3 * f1).eval(), 9. / 4);

  EXPECT_DOUBLE_EQ((f1 / 3).eval(), 1. / 4);
  EXPECT_DOUBLE_EQ((3 / f1).eval(), 4. / 1);

  EXPECT_DOUBLE_EQ((f1 - f2).eval(), -3. / 28);
  EXPECT_DOUBLE_EQ((-f2).eval(), -6. / 7);
  EXPECT_DOUBLE_EQ((f1 * f2).eval(), 9. / 14);
  EXPECT_DOUBLE_EQ((f1 / f2).eval(), 7. / 8);

  frac<int32_t> f3(3, 4);
  EXPECT_EQ(f1, f3);
  EXPECT_LT(f1, f2);
  EXPECT_NE(f1, f2);

  f1 = 0;
  EXPECT_DOUBLE_EQ(f1.eval(), 0);
}

TEST(Fraction, Advance) {}
