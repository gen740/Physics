#include <gtest/gtest.h>
#include <quantity/Quantity.h>
#include <quantity/Quantity_base.h>

using Quantity_Base::Quantity_base;

TEST(Quantity, QuantityBase) {
  // std::cout << a.value << std::endl;
  // A<AAA<4>{}> a;
  using Quantity_Base::frac;
  // constexpr frac<int> f1{1, 2);
  Quantity_base<frac{3, 5}, frac{1, 2}, frac{4, 3}, frac{3, 7}> a;
  EXPECT_EQ(a.get_L(), frac(3, 5));
  EXPECT_EQ(a.get_M(), frac(1, 2));
  EXPECT_EQ(a.get_T(), frac(4, 3));
  EXPECT_EQ(a.get_I(), frac(3, 7));
  EXPECT_TRUE(a.is_same_dim_as(
      Quantity_base<frac{6, 10}, frac{3, 6}, frac{4, 3}, frac{3, 7}>()));
}

TEST(Quantity, Basic) {
  using Quantity_Base::frac;
  Quantity_base<frac{3, 5}, frac{1, 2}, frac{4, 3}, frac{3, 7}> a1(3.4);
  Quantity_base<frac{3, 5}, frac{1, 2}, frac{4, 3}, frac{3, 7}> a2(7.4);
  Quantity_base<frac{-2}, frac{}, frac{}, frac{}> b(2.5);
  // Expect True if a1 and a2 has same dimention
  EXPECT_TRUE(a1.is_same_dim_as(a2));

  // Support for basic arithmetic operations
  EXPECT_DOUBLE_EQ((a1 + a2).value, 10.8);
  EXPECT_DOUBLE_EQ((a1 - a2).value, -4.);
  EXPECT_TRUE((a1 + a2).is_same_dim_as(a1));
  EXPECT_TRUE((a1 - a2).is_same_dim_as(a1));

  EXPECT_DOUBLE_EQ((a1 * b).value, 3.4 * 2.5);
  EXPECT_DOUBLE_EQ((a1 * 3).value, 3.4 * 3);
  EXPECT_DOUBLE_EQ((3 * a1).value, 3.4 * 3);
  EXPECT_TRUE((3 * a1).is_same_dim_as(a1));
  EXPECT_TRUE((a1 * 3).is_same_dim_as(a1));

  EXPECT_DOUBLE_EQ((a1 / b).value, 3.4 / 2.5);
  EXPECT_DOUBLE_EQ((a1 / 3).value, 3.4 / 3);
  EXPECT_DOUBLE_EQ((3. / a1).value, 3 / 3.4);
  EXPECT_TRUE((a1 / 3).is_same_dim_as(a1));
  EXPECT_TRUE((3 / a1).is_same_dim_as(
      Quantity_base<frac{-3, 5}, frac{-1, 2}, frac{-4, 3}, frac{-3, 7}>(0)));
  auto u = -a1;
  // std::cout << u.abs() << std::endl;
  // EXPECT_DOUBLE_EQ(u.abs(), a1);

  Quantity_base<frac{1}, frac{1}, frac{2}, frac{2}> a(7.);
  EXPECT_DOUBLE_EQ(a.sqrt().value, sqrt(7));
  EXPECT_DOUBLE_EQ(sqrt(a).value, sqrt(7));
  EXPECT_TRUE(sqrt(a).is_same_dim_as(
      Quantity_base<frac{1, 2}, frac{1, 2}, frac{2, 2}, frac{2, 2}>()));

  EXPECT_DOUBLE_EQ(a.cbrt().value, cbrt(7));
  EXPECT_DOUBLE_EQ(cbrt(a).value, cbrt(7));
  EXPECT_TRUE(cbrt(a).is_same_dim_as(
      Quantity_base<frac{1, 3}, frac{1, 3}, frac{2, 3}, frac{2, 3}>()));

  EXPECT_DOUBLE_EQ(a.pow<3>().value, pow(7, 3));
  EXPECT_DOUBLE_EQ(pow<3>(a).value, pow(7, 3));
  EXPECT_TRUE(pow<3>(a).is_same_dim_as(
      Quantity_base<frac{1 * 3}, frac{1 * 3}, frac{2 * 3}, frac{2 * 3}>()));

  EXPECT_DOUBLE_EQ(a.cube().value, pow(7, 3));
  EXPECT_DOUBLE_EQ(cube(a).value, pow(7, 3));
  EXPECT_TRUE(cube(a).is_same_dim_as(
      Quantity_base<frac{1 * 3}, frac{1 * 3}, frac{2 * 3}, frac{2 * 3}>()));

  Quantity_base<frac{1}, frac{1}, frac{2}, frac{2}> x(3.);
  Quantity_base<frac{1}, frac{1}, frac{2}, frac{2}> y(4.);
  Quantity_base<frac{1}, frac{1}, frac{2}, frac{2}> z(5.);
  EXPECT_DOUBLE_EQ(hypot(x, y).value, std::hypot(3, 4));
  EXPECT_TRUE(hypot(x, y).is_same_dim_as(x));
  EXPECT_DOUBLE_EQ(hypot(x, y, z).value, std::hypot(3, 4, 5));
  EXPECT_TRUE(hypot(x, y, z).is_same_dim_as(x));
  // a1 + x;
}

TEST(Quantity, Units) {
  using namespace Quantity;
  // std::cout << DimensionLess(1) << std::endl;
  // std::cout << Length(1) / Time(1) << std::endl;
  Vel v(3.4);
  EXPECT_TRUE(v.is_same_dim_as(Quantity_base<1, 0, -1, 0>{}));
}

TEST(Quantity, Literals) {
  using namespace Quantity::Literals;
  // std::cout << Quantity::Units::milli * Quantity::meter << std::endl;
  std::cout << 1.4_m << std::endl;
  std::cout << 1.4_mm << std::endl;
  // EXPECT_EQ(1_m, Quantity_base<1, 0, 0, 0>(1.0));
}
