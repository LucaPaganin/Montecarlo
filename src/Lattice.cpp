#include "Lattice.h"

void Lattice::zero_init(){
  for (size_t i = 0; i < T; i++) {
    for (size_t j = 0; j < T; j++) {
      R[i][j]=0;
    }
  }
}

void Lattice::random_init(double fill_fraction){
  this->zero_init();
  std::uniform_real_distribution<double> distr(0,1);
  if (fill_fraction<0 || fill_fraction>1){
    std::cout << "Invalid filling fraction" << std::endl;
  }
  else{
    int Nparticles = std::floor(fill_fraction*T*T);
    for (size_t k = 0; k < Nparticles; k++) {
      int i = distr(engine)*T;
      int j = distr(engine)*T;
      R[i][j] = 1;
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Lattice& L) {
  for (size_t i = 0; i < L.Size(); i++) {
    for (size_t j = 0; j < L.Size(); j++) {
      os << L.lattice()[i][j] << "\t";
    }
    os << std::endl;
  }
  return os;
}

unsigned* Lattice::get_NeighborCoordinates(int i, int j, int dir) const{
  unsigned* coords = new unsigned[2];
  if (dir < 0 || dir > 3){
    std::cout << "Invalid direction" << std::endl;
  }
  else{
    switch (dir) {
      case 0:
      coords[0]=i;
      if (j==0) {
        coords[1]=T-1;
      } else {
        coords[1]=j-1;
      }
      break;

      case 1:
      coords[1]=j;
      if (i==0) {
        coords[0]=T-1;
      } else {
        coords[0]=i-1;
      }
      break;

      case 2:
      coords[0]=i;
      if (j==T-1) {
        coords[1]=0;
      } else {
        coords[1]=j+1;
      }
      break;

      case 3:
      coords[1]=j;
      if (i==T-1) {
        coords[0]=0;
      } else {
        coords[0]=i+1;
      }
      break;
    }
  }
  return coords;
}

int Lattice::get_NeighborsNumber(int i, int j) const{
  int n=0;
  for (unsigned dir = 0; dir < 4; dir++) {
    auto neighbor_coords = this->get_NeighborCoordinates(i,j,dir);
    if (R[neighbor_coords[0]][neighbor_coords[1]] != 0) n+=1;
  }
  return n;
}
