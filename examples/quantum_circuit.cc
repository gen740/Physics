#include <QuantumCircuit/QuantumCircuit.h>

#include <iostream>

void a() {
  auto q = QuantumCircuit::QCircuit(2);

  // q.h(0);
  q.cx(0, 1);

  std::cout << q << std::endl;

  q.compile();

  std::cout << q.get_inner_repr() << std::endl;
}

void b() {
  auto q = QuantumCircuit::QCircuit(2);

  q.h(0);
  q.h(1);
  q.cx(0, 1);
  q.h(0);
  q.h(1);

  std::cout << q << std::endl;

  q.compile();

  std::cout << q.get_inner_repr() << std::endl;
}

void c() {
  auto q = QuantumCircuit::QCircuit(2);

  q.cx(0, 1);
  q.cx(1, 0);
  q.cx(0, 1);

  std::cout << q << std::endl;

  q.compile();

  std::cout << q.get_inner_repr() << std::endl;
}

void h() {
  auto q = QuantumCircuit::QCircuit(2);

  q.h(0);
  q.cx(0, 1);

  std::cout << q << std::endl;

  q.compile();

  std::cout << q.eval(0) << std::endl;
}

void g() {
  auto q = QuantumCircuit::QCircuit(2);

  q.h(1);

  q.h(0);
  q.cx(0, 1);
  q.h(0);

  std::cout << q << std::endl;

  q.compile();

  std::cout << q.eval(0) << std::endl;
}

int main() {
  // a();
  // b();
  // c();
  h();
  // g();
  return 0;
}
