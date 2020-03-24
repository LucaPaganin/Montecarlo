#include "Lattice.h"

void Lattice::zero_init(){
  for (size_t i = 0; i < dim; i++) {
    for (size_t j = 0; j < dim; j++) {
      R[i][j]=0;
    }
  }
}

void Lattice::random_init(double fill_fraction){
  this->zero_init();
  if (fill_fraction<0 || fill_fraction>1){
    std::cout << "Invalid filling fraction" << std::endl;
  }
  else{
    int Nparticles = std::floor(fill_fraction*dim*dim);
    for (size_t k = 0; k < Nparticles; k++) {
      int i = distr(engine)*dim;
      int j = distr(engine)*dim;
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

int* Lattice::get_NeighborCoordinates(int i, int j, int dir) const{
  int* coords = new int[2];
  if (dir < 0 || dir > 3){
    std::cout << "Invalid direction" << std::endl;
  }
  else{
    switch (dir) {
      case 0:
      coords[0]=i;
      if (j==0) {
        coords[1]=dim-1;
      } else {
        coords[1]=j-1;
      }
      break;

      case 1:
      coords[1]=j;
      if (i==0) {
        coords[0]=dim-1;
      } else {
        coords[0]=i-1;
      }
      break;

      case 2:
      coords[0]=i;
      if (j==dim-1) {
        coords[1]=0;
      } else {
        coords[1]=j+1;
      }
      break;

      case 3:
      coords[1]=j;
      if (i==dim-1) {
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
  for (int dir = 0; dir < 4; dir++) {
    auto neighbor_coords = this->get_NeighborCoordinates(i,j,dir);
    if (R[neighbor_coords[0]][neighbor_coords[1]] != 0) n+=1;
  }
  return n;
}

int Lattice::get_OppositeDirection(int dir) const{
  int oppdir;
  if (dir<2){
    oppdir = dir+2;
  }
  else {
    oppdir = dir-2;
  }
  return oppdir;
}

std::vector<int> Lattice::get_NearestNeighbors(int i, int j) const{
  std::vector<int> neighbors;
    for (int dir = 0; dir < 4; dir++) {
      int *coords = this->get_NeighborCoordinates(i, j, dir);
      if (R[coords[0]][coords[1]]!=0) {
        neighbors.push_back(R[coords[0]][coords[1]]);
      }
    }
  return neighbors;
}

bool Lattice::IsThere_aNeighbor(int i, int j, int dir) const{
  auto neigh_coords = this->get_NeighborCoordinates(i,j,dir);
  bool neighbor=true;
  if (R[neigh_coords[0]][neigh_coords[1]]==0) {
    neighbor=false;
  }
  return neighbor;
}
