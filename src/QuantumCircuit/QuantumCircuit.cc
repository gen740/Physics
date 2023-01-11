#include <QuantumCircuit/QuantumCircuit.h>
#include <linalg/Matrix.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace QuantumCircuit {

using Linalg::Matrix;
using Linalg::ZMatrix;

QCircuit::QCircuit(uint32_t nbit) : unit_num{1U << nbit}, num_qbit{nbit} {
  if (nbit > 32) [[unlikely]] {
    throw std::runtime_error("Quantum Circuit cannot handle nbit > 32");
  }
  inner_repr = ZMatrix::Diag(static_cast<std::complex<double>>(1), unit_num);
};

auto QCircuit::get_inner_repr() { return inner_repr; }

void QCircuit::h_(std::vector<uint32_t> t) {
  factor *= 1 / std::sqrt(2);
  auto ir_copy = inner_repr;
  for (size_t i = 0; i < unit_num; i++) {
    if ((i & (1 << t.at(0))) != 0u) {
      inner_repr.swap(i + 1, (i & ~(1 << t.at(0))) + 1);
    }
  }
  for (size_t i = 0; i < unit_num; i++) {
    for (size_t j = 0; j < unit_num; j++) {
      if (((~i & (1 << t.at(0))) != 0u) && ((~j & (1 << t.at(0))) != 0u)) {
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

void QCircuit::x_(std::vector<uint32_t> t) {
  for (size_t i = 0; i < unit_num; i++) {
    if ((i & (1 << t.at(0))) != 0u) {
      inner_repr.swap(i + 1, (i & ~(1 << t.at(0))) + 1);
    }
  }
}

void QCircuit::cx_(std::vector<uint32_t> ct) {
  for (size_t i = 0; i < unit_num; i++) {
    if ((i & (1 << ct.at(0))) != 0u && (i & (1 << ct.at(1))) != 0u) {
      inner_repr.swap(i + 1, (i & ~(1 << ct.at(1))) + 1);
    }
  }
}

void QCircuit::ccx_(std::vector<uint32_t> cct) {
  for (size_t i = 0; i < unit_num; i++) {
    if ((i & (1 << cct.at(0))) != 0u && (i & (1 << cct.at(1))) != 0u &&
        (i & (1 << cct.at(2))) != 0u) {
      inner_repr.swap(i + 1, (i & ~(1 << cct.at(2))) + 1);
    }
  }
}

void QCircuit::h(uint32_t target) {
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::H, std::vector<uint32_t>{target});
}

void QCircuit::x(uint32_t target) {
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::X, std::vector<uint32_t>{target});
}

void QCircuit::cx(uint32_t control, uint32_t target) {
  if (control >= num_qbit) [[unlikely]] {
    throw std::runtime_error("control is larger than qbit num");
  }
  if (target >= num_qbit) [[unlikely]] {
    throw std::runtime_error("target is larger than qbit num");
  }
  gates.emplace_back(Quantum_gate::CX, std::vector<uint32_t>{control, target});
}

void QCircuit::ccx(uint32_t control1, uint32_t control2, uint32_t target) {
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
                     std::vector<uint32_t>{control1, control2, target});
}

void QCircuit::compile() {
  std::cout << "Compiling ... " << std::endl;
  for (auto&& i : gates) {
    std::cout << "#" << std::flush;
    switch (i.first) {
      case Quantum_gate::H:
        h_(i.second);
        break;
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

std::vector<std::pair<uint32_t, double>> measure(const ZMatrix& result) {
  std::vector<std::pair<uint32_t, double>> ret;
  ret.reserve(result.col_size());
  for (size_t i = 1; i <= result.col_size(); i++) {
    ret.emplace_back(i - 1, std::abs(result(i, 1)));
  }
  return ret;
}

std::ostream& operator<<(std::ostream& os, const QCircuit& qc) {
  uint32_t row = 2 * qc.num_qbit - 1;
  std::vector<std::string> qc_view(row);
  for (uint32_t i = 0; i < row; i++) {
    if (i % 2 == 0) {
      qc_view.at(i) += "|" + std::to_string(i / 2) + "⟩ ";
    } else {
      qc_view.at(i) += "    ";
    }
  }
  for (uint32_t j = 0; j < row; j++) {
    if (j % 2 == 0) {
      qc_view.at(j) += "─";
    }
  }
  for (auto&& i : qc.gates) {
    uint32_t ccx_min;
    uint32_t ccx_max;
    switch (i.first) {
      case Quantum_gate::H:
        for (uint32_t j = 0; j < row; j++) {
          if (2 * i.second.at(0) == j) {
            qc_view.at(j) += "H";
          } else if (j % 2 == 0) {
            qc_view.at(j) += "─";
          } else {
            qc_view.at(j) += " ";
          }
        }
        break;
      case Quantum_gate::X:
        for (uint32_t j = 0; j < row; j++) {
          if (2 * i.second.at(0) == j) {
            qc_view.at(j) += "X";
          } else if (j % 2 == 0) {
            qc_view.at(j) += "─";
          } else {
            qc_view.at(j) += " ";
          }
        }
        break;
      case Quantum_gate::CX:
        ccx_min = 2 * *std::min_element(i.second.begin(), i.second.end());
        ccx_max = 2 * *std::max_element(i.second.begin(), i.second.end());
        for (uint32_t j = 0; j < row; j++) {
          if (2 * i.second.at(1) == j) {
            qc_view.at(j) += "X";
          } else if (j % 2 == 0 && (ccx_min < j) && (j < ccx_max)) {
            qc_view.at(j) += "┼";
          } else if ((ccx_min < j) && (j < ccx_max)) {
            qc_view.at(j) += "│";
          } else if (2 * i.second.at(0) == j) {
            qc_view.at(j) += "●";
          } else if (j % 2 == 0) {
            qc_view.at(j) += "─";
          } else {
            qc_view.at(j) += " ";
          }
        }
        break;
      case Quantum_gate::CCX:
        ccx_min = 2 * *std::min_element(i.second.begin(), i.second.end());
        ccx_max = 2 * *std::max_element(i.second.begin(), i.second.end());
        auto c1 = 2 * i.second.at(0);
        auto c2 = 2 * i.second.at(1);
        auto t = 2 * i.second.at(2);
        for (uint32_t j = 0; j < row; j++) {
          if (t == j) {
            qc_view.at(j) += "X";
          } else if (c1 == j || c2 == j) {
            qc_view.at(j) += "●";
          } else if (j % 2 == 0 && (ccx_min < j) && (j < ccx_max)) {
            qc_view.at(j) += "┼";
          } else if ((ccx_min < j) && (j < ccx_max)) {
            qc_view.at(j) += "│";
          } else if (j % 2 == 0) {
            qc_view.at(j) += "─";
          } else {
            qc_view.at(j) += " ";
          }
        }
        break;
    }
    for (uint32_t i = 0; i < row; i++) {
      if (i % 2 == 0) {
        qc_view.at(i) += "─";
      } else {
        qc_view.at(i) += " ";
      }
    }
  }
  for (uint32_t i = 0; i < row; i++) {
    if (i % 2 == 0) {
      qc_view.at(i) += " |" + std::to_string(i / 2) + "⟩";
    } else {
      qc_view.at(i) += "    ";
    }
  }
  for (auto&& i : qc_view) {
    os << i << std::endl;
  }
  return os;
}

}  // namespace QuantumCircuit
