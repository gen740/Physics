#include <QuantumCircuit/QuantumCircuit.h>

#include <algorithm>
#include <bit>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace QuantumCircuit {

std::ostream& operator<<(std::ostream& os, const QCircuit& qc) {
  uint64_t row = 2 * qc.num_qbit - 1;
  std::vector<std::string> qc_view(row);
  for (uint64_t i = 0; i < row; i++) {
    if (i % 2 == 0) {
      qc_view.at(i) += "|" + std::to_string(i / 2) + "⟩ ";
    } else {
      qc_view.at(i) += "    ";
    }
  }
  for (uint64_t j = 0; j < row; j++) {
    if (j % 2 == 0) {
      qc_view.at(j) += "─";
    } else {
      qc_view.at(j) += " ";
    }
  }
  for (auto&& i : qc.gates) {
    switch (i.first) {
      case Quantum_gate::P: {
        auto [t, p] = std::any_cast<std::tuple<uint64_t, double>>(i.second);
        // auto p =
        //     std::get<1>(std::any_cast<std::tuple<uint64_t,
        //     double>>(i.second));
        for (uint64_t j = 0; j < row; j++) {
          if (2 * t == j) {
            qc_view.at(j) += "P";
          } else if (j % 2 == 0) {
            qc_view.at(j) += "─";
          } else {
            qc_view.at(j) += " ";
          }
        }
      } break;
      case Quantum_gate::H: {
        auto [t] = std::any_cast<std::tuple<uint64_t>>(i.second);
        for (uint64_t j = 0; j < row; j++) {
          if (2 * t == j) {
            qc_view.at(j) += "H";
          } else if (j % 2 == 0) {
            qc_view.at(j) += "─";
          } else {
            qc_view.at(j) += " ";
          }
        }
      } break;
      case Quantum_gate::X: {
        auto [t] = std::any_cast<std::tuple<uint64_t>>(i.second);
        for (uint64_t j = 0; j < row; j++) {
          if (2 * t == j) {
            qc_view.at(j) += "X";
          } else if (j % 2 == 0) {
            qc_view.at(j) += "─";
          } else {
            qc_view.at(j) += " ";
          }
        }
      } break;
      case Quantum_gate::CX: {
        auto [c, t] = std::any_cast<std::tuple<uint64_t, uint64_t>>(i.second);
        auto cx_min = 2 * std::min(c, t);
        auto cx_max = 2 * std::max(c, t);
        for (uint64_t j = 0; j < row; j++) {
          if (2 * t == j) {
            qc_view.at(j) += "X";
          } else if (j % 2 == 0 && (cx_min < j) && (j < cx_max)) {
            qc_view.at(j) += "┼";
          } else if ((cx_min < j) && (j < cx_max)) {
            qc_view.at(j) += "│";
          } else if (2 * c == j) {
            qc_view.at(j) += "●";
          } else if (j % 2 == 0) {
            qc_view.at(j) += "─";
          } else {
            qc_view.at(j) += " ";
          }
        }
      } break;
      case Quantum_gate::CCX: {
        auto [c1, c2, t] =
            std::any_cast<std::tuple<uint64_t, uint64_t, uint64_t>>(i.second);
        c1 *= 2;
        c2 *= 2;
        t *= 2;
        auto ccx_min = 2 * std::min(c1, std::min(c2, t));
        auto ccx_max = 2 * std::max(c1, std::max(c2, t));
        for (uint64_t j = 0; j < row; j++) {
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
    }
    for (uint64_t i = 0; i < row; i++) {
      if (i % 2 == 0) {
        qc_view.at(i) += "─";
      } else {
        qc_view.at(i) += " ";
      }
    }
  }
  for (uint64_t i = 0; i < row; i++) {
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
