#ifndef LATTICE_H
#define LATTICE_H

#include <vector>


class Lattice{
public:
  Lattice(): R(NULL),T(0){};
  Lattice(int dim){
    T=dim;
    R = new int*[T];
    for (size_t i = 0; i < T; i++) {
      R[i] = new int[T];
    }
  }
  void zero_init();
  void random_init();
  const int Size() const{return T;}

  friend std::ostream& operator<<(std::ostream&, const Lattice&);
  friend std::istream& operator>>(std::istream&, Lattice&);

private:
  int **R;
  int T;
};







#endif
