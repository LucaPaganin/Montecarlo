#include "Lattice.h"

int main(int argc, char const *argv[]) {

  int T=10;
  Lattice L(T);
  //L.setRandomSeed(0);
  L.random_init(0.5);
  std::cout << L.Size() << std::endl;
  std::cout << L << std::endl;

  std::cout << L.get_neighbors_number(0,2) << std::endl;

  return 0;
}
