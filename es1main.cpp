#include "LatticeGas.h"
#include <map>
#include <fstream>
#include <string>
#include <memory>
#include <time.h>

int main(int argc, char const *argv[]) {
  int T=64;
  double J=1.9;
  int NMCS=10000;
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

  unsigned seed = 0;
  LatticeGas lg(T,J,seed);

  /*
  std::ofstream infofile("output/info.txt");
  infofile << "NMCS: " << NMCS << std::endl;
  infofile << "J/kB*T: " << J << std::endl;
  infofile.close();
  std::map<std::string, std::unique_ptr<std::ofstream>> outfiles;
  std::vector<std::string> filenames {"energy", "order_parameter"};
  for (const auto &name: filenames){
    outfiles[name] = std::make_unique<std::ofstream>(("output/"+name+".txt").c_str());
  }
  std::ofstream initial_config("output/initial_config.txt");
  initial_config << lg << std::endl;
  initial_config.close();
  int Nsamples = 1000;
  int M = NMCS / Nsamples;
  */

  for (unsigned i = 0; i < 1000; i++) {
    lg.MonteCarloRealization(NMCS);
    ordpar_realizations.push_back(lg.compute_OrderParameter());
  }

  auto mean_ordpar = std::accumulate(ordpar_realizations.cbegin(),
                                     ordpar_realizations.cend(), 0.0) / ordpar_realizations.size();

  std::cout << "Mean ordpar = " << mean_ordpar << std::endl;

  std::ofstream ofile("ordpar_realiz.txt");
  std::for_each(ordpar_realizations.cbegin(),ordpar_realizations.cend(),[&ofile](auto x){ofile << x << std::endl;});
  ofile.close();
  /*
  std::cout << lg.compute_OrderParameter() << std::endl;
  std::cout << lg.compute_TotalEnergy() << std::endl;
  std::ofstream final_config("output/final_config.txt");
  final_config << lg << std::endl;
  final_config.close();
  */

  return 0;
}
