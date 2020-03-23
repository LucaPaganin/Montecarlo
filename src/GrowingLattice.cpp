#include "GrowingLattice.h"

void GrowingLattice::DepositParticle(){
  int i=distr(engine)*T;
  int j=distr(engine)*T;
  while (R[i][j]!=0) {
    i = distr(engine)*T;
    j = distr(engine)*T;
  }
  particles.push_back(std::array<int,2>{i,j});
  auto last_particle_id = particles.size();
  R[i][j] = last_particle_id;
  int nneigh = this->get_NeighborsNumber(i,j);
  neighbor_classes[nneigh].push_back(last_particle_id);
}

void GrowingLattice::MoveParticle(int p, int dir){

}

void GrowingLattice::PrintNeighborClasses(std::ostream& os) const{
  for (unsigned k=0; k<5; ++k){
    os << k << ": ";
    std::for_each(neighbor_classes[k].cbegin(),
                  neighbor_classes[k].cend(),
                  [&os](auto x){os << x << " ";});
    os << std::endl;
  }
}

void GrowingLattice::InitializeClasses(){
  for (size_t c = 0; c < 5; ++c){
    neighbor_classes[c].clear();
  }
  for (size_t p = 0; p < particles.size(); p++) {
    int nneigh = this->get_NeighborsNumber(particles[p][0], particles[p][1]);
    neighbor_classes[nneigh].push_back(p+1);
  }
}

std::vector<int> GrowingLattice::get_NearestNeighbors(int p) const{
  std::vector<int> neighbors;
    for (unsigned dir = 0; dir < 4; dir++) {
      unsigned *coords = this->get_NeighborCoordinates(particles[p-1][0], particles[p-1][1], dir);
      if (R[coords[0]][coords[1]]!=0) {
        neighbors.push_back(R[coords[0]][coords[1]]);
      }
    }
  return neighbors;
}
