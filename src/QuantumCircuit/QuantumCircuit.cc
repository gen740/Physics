#include <QuantumCircuit/QuantumCircuit.h>
#include <linalg/Matrix.h>

#include <iostream>

namespace QuantumCircuit {

using Linalg::Matrix;
using Linalg::ZMatrix;

QCircuit::QCircuit(int nbit) : unit_num(1 << nbit) {
  inner_repr = ZMatrix::Diag(static_cast<std::complex<double>>(1), unit_num);
};

auto QCircuit::get_inner_repr() { return inner_repr; }

void QCircuit::x_(std::vector<uint32_t> t) {
  if (t.size() != 1) {
    throw std::runtime_error("CX should take target args");
  }
  ZMatrix cx_mat(unit_num, unit_num);
  for (uint32_t i = 0; i < unit_num; i++) {
    for (uint32_t j = 0; j < unit_num; j++) {
      auto k = j;
      if ((j & (1 << t.at(0))) != 0u) {
        k = j - (1 << t.at(0));
      } else {
        k = j + (1 << t.at(0));
      }
      cx_mat(static_cast<int>(i + 1), static_cast<int>(j + 1)) =
          static_cast<std::complex<double>>(static_cast<double>(i == k));
    }
  }
  inner_repr = cx_mat * inner_repr;
}

void QCircuit::cx_(std::vector<uint32_t> ct) {
  if (ct.size() != 2) {
    throw std::runtime_error("CX should take control and target args");
  }
  ZMatrix cx_mat(unit_num, unit_num);
  for (uint32_t i = 0; i < unit_num; i++) {
    for (uint32_t j = 0; j < unit_num; j++) {
      auto k = j;
      if ((j & (1 << ct.at(0))) != 0u) {
        if ((j & (1 << ct.at(1))) != 0u) {
          k = j - (1 << ct.at(1));
        } else {
          k = j + (1 << ct.at(1));
        }
      }
      cx_mat(static_cast<int>(i + 1), static_cast<int>(j + 1)) =
          static_cast<std::complex<double>>(static_cast<double>(i == k));
    }
  }
  inner_repr = cx_mat * inner_repr;
}

void QCircuit::ccx_(std::vector<uint32_t> cct) {
  if (cct.size() != 3) {
    throw std::runtime_error(
        "CCX should take control1, control2 and target args");
  }
  ZMatrix ccx_mat(unit_num, unit_num);
  for (uint32_t i = 0; i < unit_num; i++) {
    for (uint32_t j = 0; j < unit_num; j++) {
      auto k = j;
      if (((j & (1 << cct.at(0))) != 0u && (j & (1 << cct.at(1))) != 0u)) {
        if ((j & (1 << cct.at(2))) != 0u) {
          k = j - (1 << cct.at(2));
        } else {
          k = j + (1 << cct.at(2));
        }
      }
      ccx_mat(static_cast<int>(i + 1), static_cast<int>(j + 1)) =
          static_cast<std::complex<double>>(static_cast<double>(i == k));
    }
  }
  inner_repr = ccx_mat * inner_repr;
}

void QCircuit::x(uint32_t target) {
  gates.emplace_back(Quantum_gate::X, std::vector<uint32_t>{target});
}

void QCircuit::cx(uint32_t control, uint32_t target) {
  gates.emplace_back(Quantum_gate::CX, std::vector<uint32_t>{control, target});
}

void QCircuit::ccx(uint32_t control1, uint32_t control2, uint32_t target) {
  gates.emplace_back(Quantum_gate::CCX,
                     std::vector<uint32_t>{control1, control2, target});
}

void QCircuit::compile() {
  std::cout << "Compiling ... " << std::endl;
  for (auto&& i : gates) {
    std::cout << "#" << std::flush;
    switch (i.first) {
      case Quantum_gate::X:
        x_(i.second);
        break;
      case Quantum_gate::CX:
        cx_(i.second);
        break;
      case Quantum_gate::CCX:
        ccx_(i.second);
        break;
      default:
        break;
    }
  }
  std::cout << std::endl;
  compiled = true;
}

ZMatrix QCircuit::eval(uint32_t init) {
  if (!compiled) {
    throw std::runtime_error("Quantum circuit must compiled before evalation");
  }
  ZMatrix init_state(unit_num, 1);
  init_state(init + 1, 1) = 1;
  return inner_repr * ZMatrix(init_state);
}

ZMatrix QCircuit::eval(const ZMatrix& init_mat) {
  if (!compiled) {
    throw std::runtime_error("Quantum circuit must compiled before evalation");
  }
  return inner_repr * init_mat;
}

std::vector<std::pair<uint32_t, double>> measure(const ZMatrix& result) {
  std::vector<std::pair<uint32_t, double>> ret;
  ret.reserve(result.col_size());
  for (size_t i = 1; i <= result.col_size(); i++) {
    ret.emplace_back(i - 1, std::abs(result(i, 1)));
  }
  return ret;
}

}  // namespace QuantumCircuit
