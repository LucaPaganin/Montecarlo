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

  gl.DDAGrowth();
  //gl.GrowLattice();

  std::map<std::string, std::ofstream*> outfiles;
  std::array<std::string,2> filenames{"final_config","neighbor_classes"};
  for (const auto& n: filenames) {
    outfiles[n] = new std::ofstream(("output2/"+n+".txt").c_str());
  }
  
  gl.PrintLatticeToFile("output2/final_config.txt");
  gl.PrintNeighborClasses(*outfiles["neighbor_classes"]);
  return 0;
}
