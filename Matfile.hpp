#ifndef MATFILE_H
#define MATFILE_H

#include <vector>
#include "datafile.hpp"
#include "matio.h"
#include <string>
#include "molecule.hpp"

using namespace std;

class Matfile: public datafile {
 private:
  mat_t *matfp;
  matvar_t *matdata;
  //vector<masspoint> peakdata;
  vector<double> massaxisdata;
  vector<double> signaldata;
  bool peakdataLoaded;
  bool loadPeakdata();
  matvar_t *getElemByName(matvar_t *parent, string name);
  e_resolutionmethod resmethod;
  double resparam;
  vector<double> clist;
  vector<double> reslist;
  bool loadResmethod();
  bool resmethodLoaded;
  shape_t s;
  bool loadShape();
  bool shapeLoaded;
  vector<molecule> molecules;
  bool loadMolecules();
  bool moleculesLoaded;
  Matfile() { };
 public:
  Matfile(string filename);
  //masspoint* getPeakdata(int index);
  int getPeakdataLength();
  double *getMassaxisArr();
  double getMassaxis(int index);
  double *getSignalArr();
  double getSignal(int index);
  molecule* getMolecule(int index);
  int getMoleculeNumber();
  e_resolutionmethod getResolutionmethod();
  double getResolutionparam();
  double getResolutionlist(int index);
  double* getResolutionlistArr();
  int getResolutionlistLength();
  double getComlist(int index);
  double* getComlistArr();
  int getComlistLength();
  shape_t* getShape();
};
  
#endif
