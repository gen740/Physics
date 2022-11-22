#pragma once
#include <linalg/Matrix.h>

#include <iosfwd>
#include <vector>

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
class QCircuit {
  uint32_t unit_num{0};
  int num_qbit{0};
  bool compiled = false;

  ZMatrix inner_repr;
  std::vector<std::pair<Quantum_gate, std::vector<uint32_t>>> gates;

  void x_(std::vector<uint32_t> t);
  void cx_(std::vector<uint32_t> ct);
  void ccx_(std::vector<uint32_t> cct);

 public:
  QCircuit() = delete;
  explicit QCircuit(int nbit);
  auto get_inner_repr();

  void x(uint32_t target);
  void cx(uint32_t control, uint32_t target);
  void ccx(uint32_t control1, uint32_t control2, uint32_t target);

  void compile();
  ZMatrix eval(uint32_t init);
  ZMatrix eval(const ZMatrix& init_mat);

  friend std::ostream& operator<<(std::ostream& os, const QCircuit& qc);
};

extern std::vector<std::pair<uint32_t, double>> measure(const ZMatrix& result);

}  // namespace QuantumCircuit
