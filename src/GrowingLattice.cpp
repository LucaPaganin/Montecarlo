#include "GrowingLattice.h"

void GrowingLattice::DepositParticle(){
  int i = distr(engine)*dim;
  int j = distr(engine)*dim;
  while (R[i][j]!=0) {
    i = distr(engine)*dim;
    j = distr(engine)*dim;
  }
  particles.push_back(std::array<int,2>{i,j});
  auto last_particle = particles.size();
  R[i][j] = last_particle;
  int nneigh = this->get_NeighborsNumber(i,j);
  neighbor_classes[nneigh].push_back(last_particle);
  //Update deposited particle neighbors classes
  auto neighs = this->get_NearestNeighbors(i,j);
  for (auto& neigh: neighs){
    //
    auto npvf = this->get_NeighborsNumber(particles[neigh-1][0], particles[neigh-1][1]);
    auto npvi = npvf-1;
    this->ChangeNeighborClass(neigh,npvi,npvf);
  }
}

void GrowingLattice::MoveParticle(int p, int dir){
  auto xi = particles[p-1][0];
  auto yi = particles[p-1][1];
  auto final_coords = this->get_NeighborCoordinates(xi,yi,dir);
  auto xf = final_coords[0];
  auto yf = final_coords[1];
  //Compute old and new neighbors
  auto oldneighs = this->get_NearestNeighbors(xi,yi);
  auto newneighs = this->get_NearestNeighbors(xf,yf);
  //Remove particle p from newneighs
  newneighs.erase(std::remove_if(newneighs.begin(),newneighs.end(),[p](auto x){return x==p;}), newneighs.end());
  //Update class for particle being moved
  auto npvi = oldneighs.size();
  auto npvf = newneighs.size();
  this->ChangeNeighborClass(p,npvi,npvf);
  //Update classes for old neighbors
  for (auto& neigh: oldneighs){
    auto npvi = this->get_NeighborsNumber(particles[neigh-1][0], particles[neigh-1][1]);
    auto npvf = npvi-1;
    this->ChangeNeighborClass(neigh, npvi, npvf);
  }
  //Update classes for the new neighbors
  for (auto& neigh: newneighs){
    auto npvi = this->get_NeighborsNumber(particles[neigh-1][0],
                                          particles[neigh-1][1]);
    auto npvf = npvi+1;
    this->ChangeNeighborClass(neigh,npvi,npvf);
  }
  //Actually move particle
  std::swap(R[xi][yi],R[xf][yf]);
}

void GrowingLattice::ChangeNeighborClass(int p, int npvi, int npvf){
  neighbor_classes[npvi].erase(std::remove_if(neighbor_classes[npvi].begin(),
                                              neighbor_classes[npvi].end(),
                                              [p](auto x){return x==p;}), neighbor_classes[npvi].end());
  neighbor_classes[npvf].push_back(p);
}

void GrowingLattice::PrintNeighborClasses(std::ostream& os) const{
  for (int k=0; k<5; ++k){
    os << k << ": ";
    std::for_each(neighbor_classes[k].cbegin(),
                  neighbor_classes[k].cend(),
                  [&os](auto x){os << x << " ";});
    os << std::endl;
  }
  os << std::endl;
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

void GrowingLattice::ComputeWeights(){
  for (size_t q = 0; q < 3; q++) {
    weights[q] = nu0*neighbor_classes[q].size()*(4-q)*exp(-(E0+q*Eb)/(kB*T));
  }
  weights[4] = dim*dim*flux;
}

void GrowingLattice::GrowLattice(){
  //Deposit initial particles
  for (unsigned i=0; i<3; i++) this->DepositParticle();

  this->PrintNeighborClasses(std::cout);
  this->ComputeWeights();

  //std::for_each(weights.begin(), weights.end(), [](auto x){std::cout << x << " ";});
  //std::cout << std::endl;
  //unsigned q=0;
  //std::cout << nu0*neighbor_classes[q].size()*(4-q)*exp(-(E0+q*Eb)/(kB*T)) << std::endl;
  //std::cout << nu0*neighbor_classes[q].size()*(4-q) << std::endl;
  //std::cout << exp(-(E0+q*Eb)/(kB*T)) << std::endl;
  std::cout << flux*dim*dim << std::endl;

  /*
  auto total_weight = std::accumulate(weights.begin(), weights.end(), 0.0);
  //auto w = distr(engine)*total_weight;
  double partial_weights_sums[5];
  std::partial_sum(weights.cbegin(), weights.cend(), partial_weights_sums);
  for (auto w: weights) std::cout << w << " ";
  std::cout << std::endl;
  for (auto ws: partial_weights_sums){
    std::cout << ws << " ";
  }
  std::cout << std::endl;
  */
}
