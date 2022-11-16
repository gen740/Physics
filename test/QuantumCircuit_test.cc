#include <QuantumCircuit/QuantumCircuit.h>
#include <gtest/gtest.h>

using Linalg::ZMatrix;
using QuantumCircuit::measure;
using QuantumCircuit::QCircuit;

template <size_t NUM_BIT>
ZMatrix vec_to_zmatrix(std::vector<double> vec) {
  ZMatrix ret{NUM_BIT, 1};
  for (int i = 0; i < vec.size(); i++) {
    ret(i + 1, 1) = vec.at(i);
  }
  return ret;
}

TEST(QuantumCircuit, X) {
  QCircuit<2> q;
  q.x(0);

  q.compile();

  std::vector<uint32_t> input({0b00, 0b01, 0b10, 0b11});
  std::vector<uint32_t> output({0b01, 0b00, 0b11, 0b10});

  for (int i = 0; i < 4; i++) {
    auto out = measure(q.eval(input.at(i)));
    for (auto&& j : out) {
      if (j.second > 0.9) {
        EXPECT_FLOAT_EQ(j.first, output.at(i));
      }
    }
  }
}

TEST(QuantumCircuit, CX) {
  QCircuit<2> q;
  q.cx(1, 0);
  q.compile();

  std::vector<uint32_t> input({0b00, 0b01, 0b10, 0b11});
  std::vector<uint32_t> output({0b00, 0b01, 0b11, 0b10});

  for (int i = 0; i < 4; i++) {
    auto out = measure(q.eval(input.at(i)));
    for (auto&& j : out) {
      if (j.second > 0.9) {
        EXPECT_FLOAT_EQ(j.first, output.at(i));
      }
    }
  }
}

TEST(QuantumCircuit, CCX) {
  QCircuit<3> q;
  q.ccx(2, 1, 0);
  q.compile();

  std::vector<uint32_t> input(
      {0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111});
  std::vector<uint32_t> output(
      {0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b111, 0b110});

  for (int i = 0; i < 8; i++) {
    auto out = measure(q.eval(input.at(i)));
    for (auto&& j : out) {
      if (j.second > 0.9) {
        EXPECT_FLOAT_EQ(j.first, output.at(i));
      }
    }
  }
}
