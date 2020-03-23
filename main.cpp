#include "GrowingLattice.h"
#include <map>
#include <fstream>
#include <string>
#include <memory>
#include <time.h>

void PrintVector(const std::vector<int>& v, std::ostream& os){
  std::for_each(v.cbegin(), v.cend(), [&os](auto x){os << x << " ";});
  os << std::endl;
}

int main(int argc, char const *argv[]) {
  int T = 10;
  GrowingLattice gl(T,0);

  for (size_t i = 0; i < 50; i++) {
    gl.DepositParticle();
  }

  gl.InitializeClasses();

  std::cout << gl << std::endl;

  return 0;
}
