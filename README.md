# Physics

C++ Physics Utility package (WIP).

This package mainly cosists of three parts. linalg/fraction/quantity,

- linalg is the Linear Algebra library which using lapacke and openmp.
- fraction is the fraction class library which provide simple arthmetic operation
for fraction.
- quantity is the physical quantity library which provide operations of physical
quantity.

# How to Use
If you use cmake, please add this project as subdirectry, and use it.

# Examples
Examples are in examples directory. To build these examples.
```
mkdir build
cd build
cmake ..
make examples
```

## Linalg
Linear Algebra library

## Fraction
Fraction class.

## quantity
Physical Quantity.

```cpp
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
  Quantity_Base::Quantity_base<3, -1, -2, 0> G;
  Quantity::Mass m;
  Quantity::Mass M;
  Quantity::Length r;

  assert((G * m * M / pow<2>(r)).is_same_dim_as(Quantity::Force{}));

  return 0;
}
```

`Quantity::Literals` namespace provides a lot of literals.

|  Literals      |    physical quantity       |
| ------------- |-------------  |
|_m| meter|
|_mm| millimeter|
|_m2| square meter |
|_mm2| square millimeter |
|_m3| cube meter|
|_mm3| cube millimeter|
|_kg| kilogram|
|_sec| second|
|_s| second|
|_ms| millisecond|
|_us| microsecond|
|_ns| nanosecond|
|_sqrt_s| sqrt second|
|_A| ampere|
|_mA| milli ampere|
|_Hz| Hertz|
|_rad| radian|
|_deg| degree|
|_mN| millinewton|
|_Nm| newton meter|
|_mNm| milli newton meter|
|_uNm| micro newton meter|
|_kgm2| kg meter meter|
|_kgmm2| kg millimeter millimeter|
|_V| volt|
|_mV| milli volt|
|_ohm| ohm|
|_H| henry|
|_uH| micro henry|
|_uWb| micro weber|
|_Wb| weber|
