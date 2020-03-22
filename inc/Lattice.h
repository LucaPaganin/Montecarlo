#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <iostream>

class Lattice{
public:
  Lattice(): R(NULL),T(0),engine(0){};
  Lattice(int dim){
    T = dim;
    R = new int*[T];
    for (size_t i = 0; i < T; i++) R[i] = new int[T];
    engine.seed(0);
  }
  void zero_init();
  void setRandomSeed(int seed){engine.seed(seed);}
  void random_init(double);

  const int Size() const{return T;}
  int** lattice() const {return R;}

  unsigned* getNeighbor(int,int,int) const;
  int get_neighbors_number(int,int) const;


  friend std::ostream& operator<<(std::ostream&, const Lattice&);
  friend std::istream& operator>>(std::istream&, Lattice&);

private:
  int **R;
  int T;
  std::mt19937 engine;
};


#endif
