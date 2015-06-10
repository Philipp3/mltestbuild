#ifndef DATAFILE
#define DATAFILE

enum e_resolutionmethod {
  flat,
  polynomial,
  simplex,
  pchip,
  unknown
};

typedef struct st_masspoint {
  double mass,  signal;
} masspoint;
  
class datafile {
public:
  //Molecule[] molecules();
  //Molecule molecules(int index);
  //double[] massaxis();
  virtual double massaxis(int index) = 0;
  //double[] signal();
  virtual double signal(int index) = 0;
  virtual e_resolutionmethod resolutionmethod() = 0;
  virtual double resolutionparam() = 0;
  //double[] resolutionlist();
  virtual double resolutionlist(int index) = 0;
  virtual double comlist(int index) = 0;
  //Shape* shape();
};
  
#endif
