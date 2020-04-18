#include "GrowingLattice.h"
#include "Constants.h"

void GrowingLattice::loadFromFile(const std::string& filename){
  std::ifstream file(filename.c_str());
  file >> dim >> T >> flux >> thetalim >> Gamma;
  file.close();
  R = new int*[dim];
  for (size_t i = 0; i < dim; i++) R[i] = new int[dim];
}

void GrowingLattice::DepositParticle(int x, int y, bool is_random){
  int i=x,j=y;
  if (is_random){
    i = distr(engine)*dim;
    j = distr(engine)*dim;
    while (R[i][j]!=0) {
      i = distr(engine)*dim;
      j = distr(engine)*dim;
    }
  }

  particles.push_back(std::array<int,2>{i,j});
  auto last_particle = particles.size();
  R[i][j] = last_particle;
  int nneigh = this->get_NeighborsNumber(i,j);
  neighbor_classes[nneigh].push_back(last_particle);

  //Update deposited particle neighbors classes
  auto neighs = this->get_NearestNeighbors(i,j);
  for (auto& neigh: neighs){
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
  //And update particle position list!!!!
  particles[p-1][0] = xf; particles[p-1][1] = yf;
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
  for (auto &w: weights) w=0;

  for (size_t q = 0; q < 3; q++) {
    weights[q] = Constants::nu0*neighbor_classes[q].size()*(4-q)*exp(-(Constants::E0+q*Constants::Eb)/(Constants::kB*T));
  }
  weights[4] = dim*dim*flux;
}

unsigned GrowingLattice::get_RandomClassIndex() {
  auto total_weight = std::accumulate(weights.begin(), weights.end(), 0.0);
  double* partial_weights_sums = new double[5];
  std::partial_sum(weights.cbegin(), weights.cend(), partial_weights_sums);
  auto w = distr(engine)*total_weight;
  unsigned q=0;
  for (q = 0; q < 5; q++) {
    if(w<=partial_weights_sums[q]) break;
  }
  delete[] partial_weights_sums;
  return q;
}

void GrowingLattice::MoveRandomParticleofClass(unsigned q){
  int ip = distr(engine)*(neighbor_classes[q].size()-1);
  auto p = neighbor_classes[q][ip];
  //std::cout << ip << " " << neighbor_classes[q].size() << std::endl;
  unsigned dir = std::floor(distr(engine)*4);
  if (!(this->IsThere_aNeighbor(particles[p-1][0], particles[p-1][1], dir))){
    this->MoveParticle(p, dir);
  }
}

void GrowingLattice::GrowLattice(){
  this->zero_init();
  for (unsigned i=0; i<2; i++) this->DepositParticle(0,0,true);
  while(particles.size() <= thetalim*dim*dim){
    this->ComputeWeights();
    //std::for_each(weights.cbegin(), weights.cend(), [](auto x){std::cout << x << " ";});
    //std::cout << std::endl;
    //std::cout << *this << std::endl;
    auto q = this->get_RandomClassIndex();
    if (q==4) {
      this->DepositParticle(0,0,true);
    } else {
      this->MoveRandomParticleofClass(q);
    }
  }
}

void GrowingLattice::DDAGrowth(){
  this->zero_init();
  for (unsigned i=0; i<2; i++) this->DepositParticle(0,0,true);
  double *dda_weights = new double[2];
  dda_weights[0] = 4*Gamma*neighbor_classes[0].size();
  dda_weights[1] = dim*dim;

  while (particles.size() <= dim*dim*thetalim) {
    dda_weights[0] = 4*Gamma*neighbor_classes[0].size();
    double PCprime = dda_weights[0]+dda_weights[1];
    auto w = distr(engine)*PCprime;
    if (w<=dda_weights[0]) {
      this->MoveRandomParticleofClass(0);
    } else {
      this->DepositParticle(0,0,true);
    }
  }
}

void GrowingLattice::RemoveIsolatedParticles(){
  for (auto p: neighbor_classes[0]){
    R[particles[p-1][0]][particles[p-1][1]] = 0;
  }
  neighbor_classes[0].clear();
}

unsigned GrowingLattice::IslandCount(){
  bool end=false;
  unsigned n_islands=0;
  while (!end) {
    this->ExploreIsland();
    n_islands++;

    unsigned not_counted_particles=0;
    for (size_t i = 0; i < dim; i++) {
      for (size_t j = 0; j < dim; j++) {
        if (R[i][j]!=0 && R[i][j]!=-1) {
          not_counted_particles++;
        }
      }
    }
    if (not_counted_particles==0) end=true;
  }
  return n_islands;
}

void GrowingLattice::ExploreIsland(){
  unsigned i=0,j=0;
  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++){
      if (R[i][j]!=0 && R[i][j]!=-1){
        goto stop;
      }
    }
  }
  stop:
  auto first_particle = R[i][j];
  std::vector<int> island;
  island.push_back(first_particle);
  R[i][j]=-1;
  int d_islandsize=1;
  while (d_islandsize>0) {
    auto oldsize = island.size();
    for (unsigned k=0; k<oldsize; k++){
      auto p = island[k];
      auto neighs = this->get_NearestNeighbors(particles[p-1][0], particles[p-1][1]);
      for (auto n: neighs){
        if (n>0){
          island.push_back(n);
          R[particles[n-1][0]][particles[n-1][1]]=-1;
        }
      }
    }
    auto newsize = island.size();
    d_islandsize = newsize-oldsize;
  }
}

void GrowingLattice::Fractal_Growth(){
  this->zero_init();
  this->DepositParticle(dim/2-1,dim/2-1,false);
  this->DepositParticle(dim/2-1,dim/2,false);
  this->DepositParticle(dim/2,dim/2-1,false);
  this->DepositParticle(dim/2,dim/2,false);

  double *dda_weights = new double[2];
  dda_weights[0] = 4*Gamma*neighbor_classes[0].size();
  dda_weights[1] = dim*dim;

  while (particles.size() <= dim*dim*thetalim) {
    dda_weights[0] = 4*Gamma*neighbor_classes[0].size();
    double PCprime = dda_weights[0]+dda_weights[1];
    auto w = distr(engine)*PCprime;
    if (w<=dda_weights[0]) {
      this->MoveRandomParticleofClass(0);
    } else {
      this->DepositParticle(0,0,true);
    }
  }
}

std::vector<std::array<int,2> > GrowingLattice::countAtomsInCircles(double fraction){
  std::vector<std::array<int,2> > data;
  int upperLimit = (fraction/2) * dim;

  for (int r = 2; r < upperLimit; r++) {
    int enclosed_atoms = 0;
    for (int i = 0; i < dim; i++) {
      for (int j = 0; j < dim; j++) {
        if ( (i-dim/2)*(i-dim/2) + (j-dim/2)*(j-dim/2) <= r*r ) {
          if (R[i][j] > 0) {
            R[i][j] = -1;
            enclosed_atoms++;
          }
        }
      }
    }
    data.push_back(std::array<int,2>{r,enclosed_atoms});
  }
  return data;
}
