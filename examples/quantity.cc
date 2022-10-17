#include <quantity/Quantity.h>

#include <iostream>

using namespace Quantity::Literals;

int main() {
  // define physical quantity in SI Units
  Quantity::Length a = 1_m;
  std::cout << a << std::endl;

  auto g = 9.8_m / 1.0_s / 1.0_s;
  auto v_0 = 10_m / 1.0_s;
  auto t = 10_s;
  auto x = 1. / 2 * g * t * t - v_0 * t;

  // it guarantee quantity dimension
  static_assert(x.is_same_dim_as(Quantity::Length{}), "x is not Length");

  // and provide some basic operations

  std::cout << x << std::endl;

  // Newton's laws
  auto G = Quantity::make_quant<3, -1, -2, 0>();
  Quantity::Mass m;
  Quantity::Mass M;
  Quantity::Length r;

  // assert((G * m * M / pow<2>(r)).is_same_dim_as(Quantity::Force{}));

  Quantity::Energy energy = 1_j;
  std::cout << energy / Quantity::electron_volt << std::endl;

  return 0;
}
