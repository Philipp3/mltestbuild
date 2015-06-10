#ifndef MOLECULE_H
#define MOLECULE_H

#include "matio.h"
#include <vector>
#include <string>

using namespace std;

typedef struct st_mpp {
  double mass, area;
} molPeakPoint;

typedef struct st_shape {
  vector<double> breaks;
  vector<vector<double>> coefs;
} shape_t;

typedef struct st_molecule {
public:
  vector<molPeakPoint> peakdata;
  string name;
  double area;
  double areaerror;
} molecule;

#endif
