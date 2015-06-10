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
  std::vector<masspoint> peakdata;
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
  double massaxis(int index);
  double signal(int index);
  molecule getMolecule(int index);
  e_resolutionmethod resolutionmethod();
  double resolutionparam();
  double resolutionlist(int index);
  double comlist(int index);
  shape_t shape();
};
  
#endif
