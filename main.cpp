#include <iostream>
#include <cmath>
#include "Lattice.h"

int main(int argc, char const *argv[]) {

  int T=10;
  Lattice L(T);

  std::cout << L.Size() << std::endl;

  return 0;
}
