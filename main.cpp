#include "Lattice.h"
#include <map>
#include <fstream>
#include <string>
#include <memory>
#include <time.h>

int main(int argc, char const *argv[]) {
  int T=64;
  double J=1.8;
  int NMCS=1000;
  if (argc>1) {
    switch (argc) {
      case 2:
      NMCS = std::atoi(argv[1]);
      break;

      case 3:
      NMCS = std::atoi(argv[1]);
      J = std::atof(argv[2]);
      break;
    }
  }

  Lattice L(T,J);

  std::ofstream infofile("info.txt");
  infofile << "NMCS: " << NMCS << std::endl;
  infofile << "J/kB*T: " << J << std::endl;
  infofile.close();

  std::map<std::string, std::unique_ptr<std::ofstream>> outfiles;
  std::vector<std::string> filenames {"energy", "order_parameter"};

  for (const auto &name: filenames){
    outfiles[name] = std::make_unique<std::ofstream>((name+".txt").c_str());
  }

  L.setRandomSeed(time(NULL));
  L.random_init(0.5);

  std::ofstream initial_config("initial_config.txt");
  initial_config << L << std::endl;
  initial_config.close();

  int Nsamples = 1000;
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

  std::ofstream final_config("final_config.txt");
  final_config << L << std::endl;
  final_config.close();

  return 0;
}
