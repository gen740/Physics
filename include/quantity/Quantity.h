#pragma once
#include <numbers>

#include "Quantity_base.h"

// L, M, T, I
namespace Quantity {

template <class T>
concept floating_point = std::is_floating_point_v<T>;

using DimensionLess = Quantity_Base::Quantity_base<>;
using Length = Quantity_Base::Quantity_base<1, 0, 0, 0>;
using Mass = Quantity_Base::Quantity_base<0, 1, 0, 0>;
using Time = Quantity_Base::Quantity_base<0, 0, 1, 0>;
using SqrtTime =
    Quantity_Base::Quantity_base<0, 0, Quantity_Base::frac{1, 2}, 0>;
using Current = Quantity_Base::Quantity_base<0, 0, 0, 1>;
using Area = decltype(pow<2>(Length{}));
using Volume = decltype(pow<3>(Length{}));
using Vel = decltype(Length{} / Time{});
using Acc = decltype(Length{} / pow<2>(Time{}));
using Force = decltype(Mass{} * Acc{});
using Momentum = decltype(Mass{} * Vel{});
using Frequency = decltype(DimensionLess{} / Time{});
using Pressure = decltype(Force{} / Area{});
using Energy = decltype(Force{} * Length{});
using Power = decltype(Energy{} / Time{});
using Angle = DimensionLess;
using AngularVel = decltype(Angle{} / Time{});
using AngularAcc = decltype(Angle{} / pow<2>(Time{}));
using Inertia = decltype(Mass{} * Length{} * Length{});
using AngularMomentum = decltype(AngularVel{} * Inertia{});
using Torque = decltype(AngularAcc{} * Inertia{});
using TorqueConstant = decltype(Torque{} / Current{});
using Voltage = decltype(Power{} / Current{});
using ElectoricalCharge = decltype(Current{} * Time{});
using MagneticFlux = decltype(Voltage{} * Time{});
using Inductance = decltype(MagneticFlux{} / Current{});
using Capacitance = decltype(ElectoricalCharge{} / Voltage{});
using Resistance = decltype(Voltage{} / Current{});

// constexpr Length length = {};
// constexpr Mass mass = {};
// constexpr Time time = {};
// constexpr Current current = {};
// constexpr Area area = {};
// constexpr Volume volume = {};
// constexpr Vel vel = {};
// constexpr Acc acc = {};
// constexpr Force force = {};
// constexpr Momentum momentum = {};
// constexpr Frequency frequency = {};
// constexpr Pressure pressure = {};
// constexpr Energy energy = {};
// constexpr Power power = {};
// constexpr Angle angle = {};
// constexpr AngularVel angular_vel = {};
// constexpr AngularAcc angular_acc = {};
// constexpr Inertia inertia = {};
// constexpr AngularMomentum angular_momentum = {};
// constexpr Torque torque = {};
// constexpr TorqueConstant torque_constant = {};
// constexpr Voltage voltage = {};
// constexpr ElectoricalCharge electorical_charge = {};
// constexpr MagneticFlux magnetic_flux = {};
// constexpr Inductance inductance = {};
// constexpr Capacitance capacitance = {};
// constexpr Resistance resistance = {};

inline namespace Units {

constexpr double nano = 1e-9;
constexpr double micro = 1e-6;
constexpr double milli = 1e-3;
constexpr double centi = 1e-2;
constexpr double kilo = 1e3;
constexpr double mega = 1e6;
constexpr double giga = 1e9;

constexpr Length meter = Length(1.0);
constexpr Length millimeter = milli * meter;
constexpr Mass kg = Mass(1.0);
constexpr Time sec = Time(1.0);
constexpr SqrtTime sqrt_sec = decltype(sqrt(Time{})){1.0};
constexpr Current ampere = Current(1.0);
constexpr Angle rad = Angle(1.0);
constexpr Angle deg = Angle(std::numbers::pi / 180);
constexpr Force newton = Force(1.0);
constexpr Force millinewton = milli * newton;
constexpr Voltage volt = Voltage(1.0);
constexpr Voltage millivolt = milli * volt;
constexpr Voltage microvolt = micro * volt;
constexpr Resistance ohm = Resistance(1.0);
constexpr Resistance milliohm = milli * ohm;
constexpr Resistance microohm = micro * ohm;
constexpr Inductance henry = Inductance(1.0);
constexpr Inductance microhenry = micro * henry;
constexpr Inductance millihenry = milli * henry;
constexpr MagneticFlux weber = MagneticFlux(1.0);
constexpr MagneticFlux milliweber = milli * weber;
constexpr MagneticFlux microweber = micro * weber;

}  // namespace Units

inline namespace Literals {

#define QUANTITY_DECLARE_LITERAL(postfix, unit)                   \
  constexpr auto operator""_##postfix(long double value) {        \
    return unit * value;                                          \
  }                                                               \
  constexpr auto operator""_##postfix(unsigned long long value) { \
    return unit * value;                                          \
  }

QUANTITY_DECLARE_LITERAL(mm, millimeter)
QUANTITY_DECLARE_LITERAL(m, meter)

QUANTITY_DECLARE_LITERAL(mm2, millimeter* millimeter)
QUANTITY_DECLARE_LITERAL(m2, meter* meter)
QUANTITY_DECLARE_LITERAL(mm3, millimeter* millimeter* millimeter)
QUANTITY_DECLARE_LITERAL(m3, meter* meter* meter)
// QUANTITY_DECLARE_LITERAL(L, Volume, 1e6 * millimeter * millimeter *
// millimeter)

QUANTITY_DECLARE_LITERAL(kg, kg)
// QUANTITY_DECLARE_LITERAL(g,  1e-3 * kg)
QUANTITY_DECLARE_LITERAL(sec, sec)
QUANTITY_DECLARE_LITERAL(s, sec)
QUANTITY_DECLARE_LITERAL(ms, milli* sec)
QUANTITY_DECLARE_LITERAL(us, micro* sec)
QUANTITY_DECLARE_LITERAL(ns, nano* sec)
QUANTITY_DECLARE_LITERAL(sqrt_s, sqrt_sec)
QUANTITY_DECLARE_LITERAL(A, ampere)
QUANTITY_DECLARE_LITERAL(mA, milli* ampere)
QUANTITY_DECLARE_LITERAL(Hz, 1.0 / sec)
QUANTITY_DECLARE_LITERAL(rad, rad)
QUANTITY_DECLARE_LITERAL(deg, std::numbers::pi / 180 * rad)

// #pragma push_macro("_N")
// #undef _N
// QUANTITY_DECLARE_LITERAL(N, Force, 1e3 * millinewton)
// #pragma pop_macro("_N")

QUANTITY_DECLARE_LITERAL(mN, millinewton)
QUANTITY_DECLARE_LITERAL(Nm, newton* meter)
QUANTITY_DECLARE_LITERAL(mNm, milli* newton* meter)
QUANTITY_DECLARE_LITERAL(uNm, micro* newton* meter)
QUANTITY_DECLARE_LITERAL(kgmm2, kg* millimeter* millimeter)
QUANTITY_DECLARE_LITERAL(kgm2, kg* meter* meter)
QUANTITY_DECLARE_LITERAL(V, volt)
QUANTITY_DECLARE_LITERAL(mV, milli* volt)
QUANTITY_DECLARE_LITERAL(ohm, ohm)
QUANTITY_DECLARE_LITERAL(H, henry)
QUANTITY_DECLARE_LITERAL(mH, milli* henry)
QUANTITY_DECLARE_LITERAL(uH, micro* henry)
QUANTITY_DECLARE_LITERAL(Wb, weber)
QUANTITY_DECLARE_LITERAL(mWb, milli* weber)
QUANTITY_DECLARE_LITERAL(uWb, micro* weber)

template <Fraction::frac<> L, Fraction::frac<> M, Fraction::frac<> T,
          Fraction::frac<> I>
static inline auto make_quant(double value = 0) {
  return Quantity_Base::Quantity_base<L, M, T, I>(value);
}

}  // namespace Literals

}  // namespace Quantity
