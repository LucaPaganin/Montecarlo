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

  std::ifstream gammafile("input/gammavals.txt");
  std::vector<double> gammas, n_islands;
  double tmp;
  while (gammafile >> tmp) {
    gammas.push_back(tmp);
  }
  gammafile.close();

  for (const auto& gamma: gammas) {
    GrowingLattice gl;
    gl.loadFromFile("input/input.cfg");
    gl.zero_init();
    gl.setGamma(gamma);
    gl.DDAGrowth();
    gl.RemoveIsolatedParticles();
    auto n_isl = gl.IslandCount();
    n_islands.push_back(n_isl);
  }

  std::ofstream output("output2/n_islands_vs_gamma.txt");
  for (size_t i = 0; i < gammas.size(); i++) {
    output << gammas[i] << " " << n_islands[i] << std::endl;
  }
  output.close();
  return 0;
}
