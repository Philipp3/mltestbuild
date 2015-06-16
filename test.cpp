#include "Matfile.hpp"
#include "datafile.hpp"
#include <iostream>
#include <string>
#include <gsl/gsl_multifit.h>
#include <cstdlib>

void showData(datafile*);

void peak_width_adaption(shape_t* shape, double fwhm, double area, shape_t * result) {
  *result = *shape;
  int breaknr = result->breaks.size();
  for(int i = 0; i < breaknr; ++i)
    result->breaks[i] *= fwhm;
  int polynr = result->coefs[0].size();
  for(int i = 0; i < breaknr-1; ++i)
    for(int j = 0; j < polynr; ++j)
      result->coefs[i][j] /= pow(fwhm, polynr-j-1);
  if(fwhm < 0) {
    cout << "Error: fwhm < 0" << endl;
    exit(1);
  }
  for(int i = 0; i < breaknr-1; ++i)
    for(int j = 0; j < polynr; ++j)
      result->coefs[i][j] *= area/fwhm;

}

double peakshapeval(shape_t *shape, double mass) {
  int breaksize = shape->breaks.size();
  for(int i = 1; i < breaksize; ++i)
    if(mass < shape->breaks[i]) {
      int polynr = shape->coefs[0].size();
      double result = 0;
      for(int j = 0; j < polynr;++j)
	result += shape->coefs[i][j]*pow(mass, j);
      return result;
    }
  return -1;
}

int main(void) {
  
  Matfile f = Matfile("test.mat");
  datafile* file = &f;
  
  if(file->getResolutionmethod() != polynomial) {
    cout << "At the Moment, only polynomial Resolution is supported." << endl;
    exit(1);
  }

  int length = file->getComlistLength();
  gsl_matrix *X = gsl_matrix_alloc(length,3),
             *cov = gsl_matrix_alloc(3,3);
  gsl_vector *y = gsl_vector_alloc(length),
             *c = gsl_vector_alloc(3);
  double chisq;

  gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc(length, 3);

  for(int i = 0; i < length; ++i) {
    double xi = file->getComlist(i), yi = file->getResolutionlist(i);
    gsl_matrix_set(X, i, 0, 1.0);
    gsl_matrix_set(X, i, 1, xi);
    gsl_matrix_set(X, i, 2, xi*xi);
    gsl_vector_set(y, i, yi);
  }

  gsl_multifit_linear(X, y, c, cov, &chisq, work);
  gsl_multifit_linear_free(work);

#define C(i) (gsl_vector_get(c, (i)))
#define CALC(val) (C(0) + C(1)*(val) + C(2)*(val)*(val))
  
  cout << "Polyfit coefficients: y = " << C(2) << " x^2 + " << C(1) << " x + " << C(0) << endl;

  int l = file->getPeakdataLength();
  int n_mol = file->getMoleculeNumber();

  cout << l << " Peakdatapoints, " << n_mol << " Molecules" << endl;

  vector<vector<int>> M(l,vector<int>(n_mol,0));
  
  for(int i = 0; i < n_mol; ++i) {
    int peaknr = file->getMolecule(i)->peakdata.size();
    for(int j = 0; j < peaknr; ++j) {
      double mass = file->getMolecule(i)->peakdata[j].mass;
      double area = file->getMolecule(i)->peakdata[j].area;
      double R = CALC(mass);
      shape_t peakshape;
      peak_width_adaption(file->getShape(), mass/R, 1, &peakshape);
      int breaksize = peakshape.breaks.size();
      for(int k = 0; k < breaksize; ++k)
	peakshape.breaks[k] += mass;
      vector<int> ind(500);
      int m = 0;
      for(int k = 0; k < l; ++k)
	if(peakshape.breaks[0] < file->getMassaxis(k) && peakshape.breaks[breaksize-1] > file->getMassaxis(k)) {
	  ind[m++] = k;
	}
      for(int k = 0; k < m; ++k)
	M[ind[k]][i] += area*peakshapeval(&peakshape, file->getMassaxis(ind[k]));
      
    }
  }
  //while(true) showData(file);
  
  return 0;
}

void showData(datafile* file) {
  cout << "1: Peakshape" << endl;
  cout << "2: Resolutionmethod" << endl;
  cout << "3: Resolutionparam" << endl;
  cout << "4: Resolutionlist" << endl;
  cout << "5: Comlist" << endl;
  cout << "6: Shape" << endl;
  cout << "7: Molecules" << endl;
  cout << "Input: ";
  int input;
  cin >> input;
  switch(input) {
  case 1: {
    cout << file->getPeakdataLength() << " Data points. Enter point number: ";
    cin >> input;
    cout << "Massaxis: " << file->getMassaxis(input) << ", Signal: " << file->getSignal(input) << endl << endl;
    break;
  }
  case 2: {
    cout << "Resolutionmethod: ";
    switch(file->getResolutionmethod()) {
    case flat:
      cout << "flat";
      break;
    case polynomial:
      cout << "polynomial";
      break;
    case simplex:
      cout << "simplex";
      break;
    case pchip:
      cout << "pchip";
      break;
    case unknown:
      cout << "unknown";
      break;
    default:
      cout << "???";
    }
    cout << endl << endl;
    break;
  }
  case 3: {
    cout << "Resolutionparam: " << file->getResolutionparam() << endl << endl;
    break;
  }
  case 4: {
    cout << "Resolutionlist: " << endl;
    for(int i = 0; i < file->getResolutionlistLength(); ++i)
      cout << "\t" << file->getResolutionlist(i) << endl;
    cout << endl;
    break;
  }
  case 5: {
    cout << "Comlist: " << endl;
    for(int i = 0; i < file->getComlistLength(); ++i)
      cout << "\t" << file->getComlist(i) << endl;
    cout << endl;
    break;
  }
  case 6: {
    cout << "Shape: ";
    cout << file->getShape()->breaks.size() << " Breaks, " << file->getShape()->coefs.size() << " Coefs with " << file->getShape()->coefs[0].size() << " Coefficients." << endl << endl;
    break;
  }
  case 7: {
    cout << file->getMoleculeNumber() << " Molecules found." << endl;
    cout << "Enter Molecule number (starting at 0): ";
    cin >> input;
    cout << "Molecule data:" << endl;
    cout << "Peakdata:  " << file->getMolecule(input)->peakdata.size() << " Points." << endl;
    cout << "Name:      " << file->getMolecule(input)->name << endl;
    cout << "Area:      " << file->getMolecule(input)->area << endl;
    cout << "Areaerror: " << file->getMolecule(input)->areaerror << endl;
    cout << endl;
    break;
  }
  case 0:
    exit(0);
    break;
  default:
    cout << "Please enter a valid Number" << endl << endl;
  }
}
