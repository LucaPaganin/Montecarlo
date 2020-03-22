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

double Lattice::compute_TotalEnergy() const{
  int N=0;
  for (size_t i = 0; i < T; i++) {
    for (size_t j = 0; j < T; j++) {
      if(R[i][j]!=0){
        N += this->get_NeighborsNumber(i,j);
      }
    }
  }
  return J*N;
}

double Lattice::compute_OrderParameter() const {
  double order_par=0;
  int Na=0, Nb=0;
  for (size_t i = 0; i < T; i++) {
    for (size_t j = 0; j < T; j++) {
      if (R[i][j]!=0) {
        int tmp = i+j;
        if (tmp%2==0) {
          Na++;
        } else {
          Nb++;
        }
      }
    }
  }
  if (Na+Nb>0) {
    order_par = (double)(Na-Nb)/(Na+Nb);
  }
  return order_par;
}

void Lattice::MetropolisHastingsStep(){
  std::uniform_real_distribution<double> distr(0,1);
  int i = distr(engine)*T;
  int j = distr(engine)*T;
  while (R[i][j]==0) {
    i = distr(engine)*T;
    j = distr(engine)*T;
  }
  int dir = distr(engine)*4;
  auto final_coords = this->get_NeighborCoordinates(i,j,dir);
  if (R[final_coords[0]][final_coords[1]]==0) {
    auto ni = this->get_NeighborsNumber(i,j);
    auto nf = this->get_NeighborsNumber(final_coords[0],final_coords[1]) - 1;
    double dE = J*(nf-ni);
    if (dE<0) {
      R[final_coords[0]][final_coords[1]] = 1;
      R[i][j] = 0;
    }
    else{
      double w = distr(engine);
      if (w < std::exp(-dE)) {
        R[final_coords[0]][final_coords[1]] = 1;
        R[i][j] = 0;
      }
    }
  }
}

void Lattice::PrintData(const std::string& kind, std::ostream& os) const{
  if (kind=="energy") {
    os << this->compute_TotalEnergy() << std::endl;
  }
  else if (kind=="order_parameter"){
    os << this->compute_OrderParameter() << std::endl;
  }
}
