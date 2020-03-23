#ifndef GROWINGLATTICE_H
#define GROWINGLATTICE_H

#include "Lattice.h"
#include <array>
#include <vector>
#include <algorithm>

class GrowingLattice: public Lattice{
public:
  GrowingLattice(): Lattice() {;}
  GrowingLattice(int dim, unsigned seed): Lattice(dim,seed) {;}

  const std::vector<std::array <int,2>>& get_Particles() const {return particles;}
  const std::array<std::vector<int>,5>& get_NeighborClasses() const {return neighbor_classes;}
  const int get_NumParticles() const{return particles.size();}
  std::vector<int> get_NearestNeighbors(int) const;

  void PrintNeighborClasses(std::ostream&) const;
  void PrintParticles() const;

  void InitializeClasses();
  void DepositParticle();
  void MoveParticle(int,int);

protected:
  std::vector<std::array <int,2>> particles;
  std::array<std::vector<int>,5> neighbor_classes;
  double weights[5];
};














#endif
