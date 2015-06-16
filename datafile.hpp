#ifndef DATAFILE
#define DATAFILE

#include "molecule.hpp"

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
  //molecule[] molecules();
  virtual molecule* getMolecule(int index) = 0;
   virtual int getMoleculeNumber() = 0;
  virtual int getPeakdataLength() = 0;
  //virtual masspoint* getPeakdata(int index) = 0;
  virtual double* getMassaxisArr() = 0;
  virtual double* getSignalArr() = 0;
  virtual double getMassaxis(int index) = 0;
  virtual double getSignal(int index) = 0;
  virtual e_resolutionmethod getResolutionmethod() = 0;
  virtual double getResolutionparam() = 0;
  //double[] resolutionlist();
  virtual double getResolutionlist(int index) = 0;
  virtual double* getResolutionlistArr() = 0;
  virtual int getResolutionlistLength() = 0;
  virtual double getComlist(int index) = 0;
  virtual double* getComlistArr() = 0;
  virtual int getComlistLength() = 0;
  virtual shape_t* getShape() = 0;
};
  
#endif
