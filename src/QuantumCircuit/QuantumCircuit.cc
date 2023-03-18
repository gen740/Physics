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

QCircuit::QCircuit(uint64_t nbit) : unit_num{1U << nbit}, num_qbit{nbit} {
  if (nbit > 32) [[unlikely]] {
    throw std::runtime_error("Quantum Circuit cannot handle nbit > 32");
  }
  inner_repr = ZMatrix::Diag(static_cast<std::complex<double>>(1), unit_num);
};

ZMatrix QCircuit::get_inner_repr() { return inner_repr ; }

void QCircuit::compile() {
  std::cout << "Compiling ... " << std::endl;
  for (auto&& i : gates) {
    std::cout << "#" << std::flush;
    switch (i.first) {
      case Quantum_gate::P:
        p_(std::any_cast<std::tuple<uint64_t, double>>(i.second));
        break;
      case Quantum_gate::RX:
        rx_(std::any_cast<std::tuple<uint64_t, double>>(i.second));
        break;
      case Quantum_gate::RY:
        ry_(std::any_cast<std::tuple<uint64_t, double>>(i.second));
        break;
      case Quantum_gate::H:
        h_(std::any_cast<std::tuple<uint64_t>>(i.second));
        break;
      case Quantum_gate::X:
        x_(std::any_cast<std::tuple<uint64_t>>(i.second));
        break;
      case Quantum_gate::CX:
        cx_(std::any_cast<std::tuple<uint64_t, uint64_t>>(i.second));
        break;
      case Quantum_gate::CCX:
        ccx_(std::any_cast<std::tuple<uint64_t, uint64_t, uint64_t>>(i.second));
        break;
      default:
        break;
    }
  }
  std::cout << std::endl;
  compiled = true;
}

ZMatrix QCircuit::eval(uint64_t init) {
  if (!compiled) [[unlikely]] {
    throw std::runtime_error("Quantum circuit must compiled before evalation");
  }
  ZMatrix init_state(unit_num, 1);
  init_state(init + 1, 1) = 1;
  if (factor != 1) {
    for (size_t i = 0; i < unit_num; i++) {
      for (size_t j = 0; j < unit_num; j++) {
        inner_repr[i][j] *= factor;
      }
    }
    factor = 1;
  }
  return inner_repr * ZMatrix(init_state);
}

ZMatrix QCircuit::eval(const ZMatrix& init_mat) {
  if (!compiled) [[unlikely]] {
    throw std::runtime_error("Quantum circuit must compiled before evalation");
  }
  if (factor != 1) {
    for (size_t i = 0; i < unit_num; i++) {
      for (size_t j = 0; j < unit_num; j++) {
        inner_repr[i][j] *= factor;
      }
    }
    factor = 1;
  }
  return inner_repr * init_mat;
}

std::vector<std::pair<uint64_t, double>> measure(const ZMatrix& result) {
  std::vector<std::pair<uint64_t, double>> ret;
  ret.reserve(result.col_size());
  for (size_t i = 1; i <= result.col_size(); i++) {
    ret.emplace_back(i - 1, std::abs(result(i, 1)));
  }
  return ret;
}

}  // namespace QuantumCircuit
