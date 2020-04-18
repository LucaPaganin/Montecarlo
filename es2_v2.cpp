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

  GrowingLattice gl;
  gl.loadFromFile("input/input.cfg");
  gl.setSeed(time(NULL));

  gl.DDAGrowth();

  std::ofstream output("output2/matrix.txt");
  output << gl << std::endl;
  output.close();
  return 0;
}
