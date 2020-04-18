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
  gl.Fractal_Growth();
  gl.PrintLatticeToFile("output2/fractal.txt");

  auto data = gl.countAtomsInCircles(0.8);
  std::ofstream ofile("output2/n_vs_r.txt");
  for (const auto& arr: data){
    ofile << arr[0] << " " << arr[1] << std::endl;
  }
  ofile.close();
  return 0;
}
