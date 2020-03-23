#include "LatticeGas.h"

double LatticeGas::compute_TotalEnergy() const{
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

double LatticeGas::compute_OrderParameter() const {
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

void LatticeGas::MetropolisHastingsStep(){
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

void LatticeGas::PrintData(const std::string& kind, std::ostream& os) const{
  if (kind=="energy") {
    os << this->compute_TotalEnergy() << std::endl;
  }
  else if (kind=="order_parameter"){
    os << this->compute_OrderParameter() << std::endl;
  }
}
