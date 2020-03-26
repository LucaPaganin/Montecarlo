#ifndef LATTICEGAS_H
#define LATTICEGAS_H

#include "Lattice.h"

class LatticeGas: public Lattice{
public:
  LatticeGas(): Lattice(){;}
  LatticeGas(int dim, double coupling, unsigned seed): Lattice(dim,seed),J(coupling){;}

  double compute_OrderParameter() const;
  double compute_TotalEnergy() const;

  void MetropolisHastingsStep();
  void MonteCarloRealization(unsigned);
  void PrintData(const std::string&, std::ostream&) const;

protected:
  double J;
};






#endif
