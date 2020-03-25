#ifndef LATTICE_H
#define LATTICE_H

#include <random>
#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>

class Lattice{
public:
  Lattice(): R(NULL),dim(0),engine(0),distr(0,1){};
  Lattice(int dimension, unsigned seed){
    dim = dimension;
    R = new int*[dim];
    for (size_t i = 0; i < dim; i++) R[i] = new int[dim];
    engine.seed(seed);
  }
  void zero_init();
  void random_init(double);

  const int Size() const {return dim;}
  int** lattice() const  {return R;}

  int* get_NeighborCoordinates(int,int,int) const;
  int get_NeighborsNumber(int,int) const;
  int get_OppositeDirection(int dir) const;
  std::vector<int> get_NearestNeighbors(int,int) const;
  bool IsThere_aNeighbor(int,int,int) const;

  friend std::ostream& operator<<(std::ostream&, const Lattice&);
  friend std::istream& operator>>(std::istream&, Lattice&);

  void PrintLatticeToFile(const std::string&) const;

protected:
  int **R;
  int dim;
  std::mt19937 engine;
  std::uniform_real_distribution<double> distr;
};


#endif
