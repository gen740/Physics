#include <gtest/gtest.h>
#include <quantity/Quantity.h>

TEST(Quantity, Quantity_base) {
  // std::cout << a.value << std::endl;
  // A<AAA<4>{}> a;
  using Fraction::frac;
  // constexpr frac<int> f1{1, 2);
  Quantity_base<frac<int>{3, 5}, frac<int>{1, 2}, frac<int>{4, 3},
                frac<int>{3, 7}>
      a;
  Quantity_base<frac<int>{}, frac<int>{}, frac<int>{}, frac<int>{}> b;
  EXPECT_EQ(a.get_L(), frac<int>(3, 5));
  EXPECT_EQ(a.get_M(), frac<int>(1, 2));
  EXPECT_EQ(a.get_T(), frac<int>(4, 3));
  EXPECT_EQ(a.get_I(), frac<int>(3, 7));
}

TEST(Quantity, Basic) {
  // std::cout << a.value << std::endl;
  // A<AAA<4>{}> a;
  using Fraction::frac;
  // constexpr frac<int> f1{1, 2);
  Quantity_base<frac<int>{3, 5}, frac<int>{1, 2}, frac<int>{4, 3},
                frac<int>{3, 7}>
      a(3.4);
  Quantity_base<frac<int>{3, 5}, frac<int>{1, 2}, frac<int>{4, 3},
                frac<int>{3, 7}>
      a2(7.4);
  Quantity_base<frac<int>{}, frac<int>{}, frac<int>{}, frac<int>{}> b;
  std::cout << a + a2 << std::endl;
  // EXPECT_EQ((a * b).get_L(), frac<int>(3, 5));
  EXPECT_EQ(a.get_M(), frac<int>(1, 2));
  EXPECT_EQ(a.get_T(), frac<int>(4, 3));
  EXPECT_EQ(a.get_I(), frac<int>(3, 7));
  std::cout << a * Quantity_base<>(3) << std::endl;
}
