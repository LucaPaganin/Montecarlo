#include "Lattice.h"
#include <map>
#include <fstream>
#include <string>
#include <memory>
#include <time.h>

int main(int argc, char const *argv[]) {
  int T=10;
  double J=1;
  Lattice L(T,J);
  int NMCS=1000;

  if (argc>1) {
    NMCS = std::atoi(argv[1]);
  }

  std::map<std::string, std::unique_ptr<std::ofstream>> outfiles;

  std::vector<std::string> filenames {"energy", "order_parameter"};

  for (const auto &name: filenames){
    outfiles[name] = std::make_unique<std::ofstream>((name+".txt").c_str());
  }

  L.setRandomSeed(time(NULL));
  L.random_init(0.5);
  std::cout << L.Size() << std::endl;
  std::cout << L << std::endl;
  int Nsamples = 100;
  int M = NMCS / Nsamples;

  for (size_t i = 0; i < NMCS; i++) {
    if (i%M==0){
      for (auto it=outfiles.begin(); it!=outfiles.end(); it++){
        L.PrintData(it->first, *(it->second));
      }
    }
    L.MetropolisHastingsStep();
  }

  std::cout << L.compute_OrderParameter() << std::endl;
  std::cout << L.compute_TotalEnergy() << std::endl;

  std::cout << L << std::endl;

  return 0;
}
