#include <QuantumCircuit/QuantumCircuit.h>
#include <linalg/Matrix.h>

#include <algorithm>
#include <bit>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace QuantumCircuit {

using Linalg::Matrix;
using Linalg::ZMatrix;

void QCircuit::p_(std::tuple<uint64_t, double> tp) {
  auto phase = std::exp(std::get<1>(tp) * std::imag(1));
  // auto phase = 1;
  for (size_t i = 0; i < unit_num; i++) {
    for (size_t j = 0; j < unit_num; j++) {
      if (((i & (1 << std::get<0>(tp))) != 0u) &&
          ((j & (1 << std::get<0>(tp))) != 0u)) {
        inner_repr[i][j] *= phase;
      }
    }
  }
}

void QCircuit::rx_(std::tuple<uint64_t, double> tp) {
  auto [t, p] = tp;
  double diag = std::cos(p / 2);
  double nondiag = -std::imag(1.) * std::sin(p / 2);
  for (size_t i = 0; i < unit_num; i++) {
    for (size_t j = 0; j < unit_num; j++) {
      if (((i & (1 << t)) != 0u) == ((j & (1 << t)) != 0u)) {
        inner_repr[i][j] *= diag;
      } else {
        inner_repr[i][j] *= nondiag;
      }
    }
  }
}

void QCircuit::ry_(std::tuple<uint64_t, double> tp) {
  auto [t, p] = tp;
  double diag = std::cos(p / 2);
  double nondiag = std::sin(p / 2);
  for (size_t i = 0; i < unit_num; i++) {
    for (size_t j = 0; j < unit_num; j++) {
      if (((i & (1 << t)) != 0u) == ((j & (1 << t)) != 0u)) {
        inner_repr[i][j] *= diag;
      } else {
        inner_repr[i][j] *= ((i & (1 << t)) != 0u) ? nondiag : -nondiag;
      }
    }
  }
}

void QCircuit::h_(std::tuple<uint64_t> t) {
  factor *= 1 / std::sqrt(2);
  auto ir_copy = inner_repr;
  auto [target] = t;
  for (size_t i = 0; i < unit_num; i++) {
    if ((i & (1 << target)) != 0u) {
      inner_repr.swap(i + 1, (i & ~(1 << target)) + 1);
    }
  }
  for (size_t i = 0; i < unit_num; i++) {
    for (size_t j = 0; j < unit_num; j++) {
      if (((i & (1 << target)) != 0u) && ((j & (1 << target)) != 0u)) {
        ir_copy[i][j] *= -1;
      }
    }
  }
  for (size_t i = 0; i < unit_num; i++) {
    for (size_t j = 0; j < unit_num; j++) {
      inner_repr[i][j] += ir_copy[i][j];
    }
  }
}

void QCircuit::x_(std::tuple<uint64_t> t) {
  for (size_t i = 0; i < unit_num; i++) {
    if ((i & (1 << std::get<0>(t))) != 0u) {
      inner_repr.swap(i + 1, (i & ~(1 << std::get<0>(t))) + 1);
    }
  }
}

void QCircuit::cx_(std::tuple<uint64_t, uint64_t> ct) {
  for (size_t i = 0; i < unit_num; i++) {
    if ((i & (1 << std::get<0>(ct))) != 0u &&
        (i & (1 << std::get<1>(ct))) != 0u) {
      inner_repr.swap(i + 1, (i & ~(1 << std::get<1>(ct))) + 1);
    }
  }
}

void QCircuit::ccx_(std::tuple<uint64_t, uint64_t, uint64_t> cct) {
  for (size_t i = 0; i < unit_num; i++) {
    if ((i & (1 << std::get<0>(cct))) != 0u &&
        (i & (1 << std::get<1>(cct))) != 0u &&
        (i & (1 << std::get<2>(cct))) != 0u) {
      inner_repr.swap(i + 1, (i & ~(1 << std::get<2>(cct))) + 1);
    }
  }
}

void QCircuit::p(uint64_t target, double phase) {
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::H, std::make_tuple(target, phase));
}

void QCircuit::rx(uint64_t target, double phase) {
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::RX, std::make_tuple(target, phase));
}

void QCircuit::ry(uint64_t target, double phase) {
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::RY, std::make_tuple(target, phase));
}

void QCircuit::h(uint64_t target) {
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::H, std::make_tuple(target));
}

void QCircuit::x(uint64_t target) {
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::X, std::make_tuple(target));
}

void QCircuit::cx(uint64_t control, uint64_t target) {
  if (control >= num_qbit) [[unlikely]] {
    throw std::runtime_error("control is larger than qbit num");
  }
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::CX, std::make_tuple(control, target));
}

void QCircuit::ccx(uint64_t control1, uint64_t control2, uint64_t target) {
  if (control1 >= num_qbit) [[unlikely]] {
    throw std::runtime_error("control1 is larger than qbit num");
  }
  if (control2 >= num_qbit) [[unlikely]] {
    throw std::runtime_error("control2 is larger than qbit num");
  }
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::CCX,
                     std::make_tuple(control1, control2, target));
}

}  // namespace QuantumCircuit
