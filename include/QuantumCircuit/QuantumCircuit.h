#pragma once

#include <linalg/Matrix.h>

#include <iostream>

namespace QuantumCircuit {

using Linalg::Matrix;
using Linalg::ZMatrix;

/*! \enum name
 *
 *  Detailed description
 */
enum Quantum_gate {
  X,
  CX,
  CCX,
};

/*!
 *  この量子回路は内部表現に 32bit を使っているので、 Qbit は 32 bit 表現まで
 *  しかできないが、（2^32 = 4,294,967,296) であるから 32 bit で十分である。
 */
template <size_t NUM_BIT>
class QCircuit {
  uint32_t UNIT_NUM = 1 << NUM_BIT;
  bool compiled = false;

  ZMatrix inner_repr;
  std::vector<std::pair<Quantum_gate, std::vector<uint32_t>>> gates;

  void _x(std::vector<uint32_t> t) {
    if (t.size() != 1) {
      throw std::runtime_error("CX should take target args");
    }
    ZMatrix cx_mat(UNIT_NUM, UNIT_NUM);
    for (uint32_t i = 0; i < UNIT_NUM; i++) {
      for (uint32_t j = 0; j < UNIT_NUM; j++) {
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

  void _cx(std::vector<uint32_t> ct) {
    if (ct.size() != 2) {
      throw std::runtime_error("CX should take control and target args");
    }
    ZMatrix cx_mat(UNIT_NUM, UNIT_NUM);
    for (uint32_t i = 0; i < UNIT_NUM; i++) {
      for (uint32_t j = 0; j < UNIT_NUM; j++) {
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

  void _ccx(std::vector<uint32_t> cct) {
    if (cct.size() != 3) {
      throw std::runtime_error(
          "CCX should take control1, control2 and target args");
    }
    ZMatrix ccx_mat(UNIT_NUM, UNIT_NUM);
    for (uint32_t i = 0; i < UNIT_NUM; i++) {
      for (uint32_t j = 0; j < UNIT_NUM; j++) {
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

 public:
  explicit QCircuit() {
    inner_repr = ZMatrix::Diag(static_cast<std::complex<double>>(1), UNIT_NUM);
  };

  ZMatrix eval(uint32_t init) {
    if (!compiled) {
      throw std::runtime_error(
          "Quantum circuit must compiled before evalation");
    }
    ZMatrix init_state(UNIT_NUM, 1);
    init_state(init + 1, 1) = 1;
    return inner_repr * ZMatrix(init_state);
  }

  ZMatrix eval(const ZMatrix& init_mat) {
    if (!compiled) {
      throw std::runtime_error(
          "Quantum circuit must compiled before evalation");
    }
    return inner_repr * init_mat;
  }

  auto get_inner_repr() { return inner_repr; }

  void x(uint32_t target) {
    gates.emplace_back(Quantum_gate::X, std::vector<uint32_t>{target});
  }

  void cx(uint32_t control, uint32_t target) {
    gates.emplace_back(Quantum_gate::CX,
                       std::vector<uint32_t>{control, target});
  }

  void ccx(uint32_t control1, uint32_t control2, uint32_t target) {
    gates.emplace_back(Quantum_gate::CCX,
                       std::vector<uint32_t>{control1, control2, target});
  }

  void compile() {
    int gate_num = gates.size();
    // for (int i = 1; i <= gates.size(); i++) {
    // for (int i = 0; i < gates.size(); i++) {
    std::cout << "Compiling ... " << std::endl;
    for (auto&& i : gates) {
      std::cout << "#" << std::flush;
      switch (i.first) {
        case Quantum_gate::X:
          _x(i.second);
          break;
        case Quantum_gate::CX:
          _cx(i.second);
          break;
        case Quantum_gate::CCX:
          _ccx(i.second);
          break;
        default:
          break;
      }
    }
    std::cout << std::endl;
    compiled = true;
  }
};

std::vector<std::pair<uint32_t, double>> measure(const ZMatrix& result) {
  std::vector<std::pair<uint32_t, double>> ret;
  ret.reserve(result.col_size());
  for (int i = 1; i <= result.col_size(); i++) {
    ret.emplace_back(i - 1, std::abs(result(i, 1)));
  }
  return ret;
}

}  // namespace QuantumCircuit
