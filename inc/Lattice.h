#ifndef LATTICE_H
#define LATTICE_H

#include <random>
#include <chrono>
#include <cmath>
#include <iostream>

class Lattice{
public:
  Lattice(): R(NULL),T(0),engine(0),distr(0,1){};
  Lattice(int dim, unsigned seed){
    T = dim;
    R = new int*[T];
    for (size_t i = 0; i < T; i++) R[i] = new int[T];
    engine.seed(seed);
  }
  void zero_init();
  void random_init(double);

  const int Size() const {return T;}
  int** lattice() const  {return R;}

  unsigned* get_NeighborCoordinates(int,int,int) const;
  int get_NeighborsNumber(int,int) const;

  friend std::ostream& operator<<(std::ostream&, const Lattice&);
  friend std::istream& operator>>(std::istream&, Lattice&);

protected:
  int **R;
  int T;
  std::mt19937 engine;
  std::uniform_real_distribution<double> distr;
};


#endif
