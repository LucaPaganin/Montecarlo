#ifndef GROWINGLATTICE_H
#define GROWINGLATTICE_H

#include "Lattice.h"
/*
#define kB 8.61733238e-5
#define nu0 1e12
#define E0 0.4
#define Eb 0.2
*/

class GrowingLattice: public Lattice{
public:
  GrowingLattice(): Lattice(),flux(1./60),T(100.0),thetalim(0.1) {;}
  GrowingLattice(int dim, unsigned seed): Lattice(dim,seed),flux(1./60),T(200.0),thetalim(0.1) {;}

  double getGamma() const{return Gamma;}
  void setGamma(double gamma) {Gamma=gamma;}

  const std::vector<std::array <int,2>>& get_Particles() const {return particles;}
  const std::array<std::vector<int>,5>& get_NeighborClasses() const {return neighbor_classes;}
  const int get_NumParticles() const{return particles.size();}
  void setSeed(unsigned seed){engine.seed(seed);}
  void loadFromFile(const std::string&);

  void PrintNeighborClasses(std::ostream&) const;
  void PrintParticles() const;
  void ChangeNeighborClass(int,int,int);

  void InitializeClasses();
  void ComputeWeights();
  unsigned get_RandomClassIndex();
  void DepositParticle(int,int,bool);
  void MoveParticle(int,int);
  void MoveRandomParticleofClass(unsigned);
  void GrowLattice();
  void DDAGrowth();
  void Fractal_Growth();

  void RemoveIsolatedParticles();
  void ExploreIsland();
  unsigned IslandCount();
  std::vector<std::array<int,2> > countAtomsInCircles(double);

protected:
  std::vector<std::array <int,2>> particles;
  std::array<std::vector<int>,5> neighbor_classes;
  std::array<double,5> weights;
  double flux, T, thetalim, Gamma;
};














#endif
