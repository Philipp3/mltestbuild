#include "Matfile.hpp"
#include <string>
#include <iostream>
#include "matio.h"

using namespace std;

Matfile::Matfile(string filename) {
  matfp = Mat_Open(filename.c_str(), MAT_ACC_RDONLY);
  matdata = Mat_VarRead(matfp, "data");
  peakdataLoaded = false;
  resmethodLoaded = false;
  shapeLoaded = false;
  moleculesLoaded = false;
}

double Matfile::massaxis(int index) {
  if(!peakdataLoaded)
    loadPeakdata();
  return peakdata[index].mass;
}

double Matfile::signal(int index) {
  if(!peakdataLoaded)
    loadPeakdata();
  return peakdata[index].signal;
}

e_resolutionmethod Matfile::resolutionmethod() {
  if(!resmethodLoaded)
    loadResmethod();
  return resmethod;
}

double Matfile::resolutionparam() {
  if(!resmethodLoaded)
    loadResmethod();
  return resparam;
}

double Matfile::resolutionlist(int index) {
  if(!resmethodLoaded)
    loadResmethod();
  return reslist[index];
}

double Matfile::comlist(int index) {
  if(!resmethodLoaded)
    loadResmethod();
  return clist[index];
}

bool Matfile::loadResmethod() {
  matvar_t *resmethod_p = getElemByName(getElemByName(matdata, "calibration"), "resolutionmethode");
  if(NULL == resmethod_p)
   return false;
  string name = (char *) resmethod_p->data;
  if("Polynomial" == name)
    resmethod = polynomial;
  else if("Flat" == name)
    resmethod = flat;
  else if("Simplex" == name)
    resmethod = simplex;
  else if("Pchip" == name)
    resmethod = pchip;
  else
    resmethod = unknown;

  resmethod_p = getElemByName(getElemByName(matdata, "calibration"), "resolutionparam");
  if(NULL == resmethod_p)
    resparam = 0;
  else
    resparam = ((double *) (resmethod_p->data))[0];

  resmethod_p = getElemByName(getElemByName(matdata, "calibration"), "comlist");
  int listlen = resmethod_p->dims[1];
  //clist=new vector<double>(listlen);
  clist.resize(listlen,0.);
  for(int i = 0;i < listlen;++i) {
    clist[i] = ((double *) (resmethod_p->data))[i];
  }

  resmethod_p = getElemByName(getElemByName(matdata, "calibration"), "resolutionlist");
  listlen = resmethod_p->dims[1];
  //reslist=new vector<double>(listlen);
  reslist.resize(listlen,0.);
  for(int i = 0;i < listlen;++i) {
    reslist[i] = ((double *) (resmethod_p->data))[i];
  }
  
  resmethodLoaded = true;
  return true;
}

bool Matfile::loadPeakdata() {
  matvar_t *peakdata_p = getElemByName(matdata, "peakdata");
  if(peakdata_p != NULL) {
    int listlen = peakdata_p->dims[0];
    //peakdata = new vector<massdata>(listlen);
    peakdata.resize(listlen);
    for(int i = 0;i < listlen; ++i) {
      peakdata[i] = {((double *)(peakdata_p->data))[i], ((double *)(peakdata_p->data))[i+listlen]};
    }
    peakdataLoaded = true;
  }
  return true;
}

shape_t Matfile::shape() {
  if(!shapeLoaded)
    loadShape();
  return s;
}

bool Matfile::loadShape() {
  matvar_t *shape_p = getElemByName(getElemByName(matdata, "calibration"),"shape");
  if(NULL == shape_p)
    return false;
  matvar_t *breaks_p = getElemByName(shape_p, "breaks");
  int listlen = breaks_p->dims[1];
  s.breaks.resize(listlen);
  for(int i = 0; i < listlen; ++i)
    s.breaks[i] = ((double *)(breaks_p->data))[i];
  
  matvar_t *coefs_p = getElemByName(shape_p, "coefs");
  listlen = coefs_p->dims[0];
  s.coefs.resize(listlen, vector<double>(coefs_p->dims[1]));
  for(int i = 0, listlen2 = coefs_p->dims[1]; i < listlen; ++i)
    for(int j = 0; j < listlen2;++j)
      s.coefs[i][j] = ((double *)(coefs_p->data))[i*listlen+j];
  return true;
}

molecule Matfile::getMolecule(int index) {
  if(!moleculesLoaded)
    loadMolecules();
  return molecules[index];
}

bool Matfile::loadMolecules() {
  matvar_t *molecule_p = getElemByName(matdata, "molecules");
  if(NULL == molecule_p) {
    cout << "Molecule data not found." << endl;
    return false;
  }
  int molNumber = molecule_p->dims[1];
  int fieldNumber = Mat_VarGetNumberOfFields(molecule_p);
  cout << "molNumber: " << molNumber << ", fieldNumber: " << fieldNumber << endl;
  molecules.resize(molNumber);
  matvar_t **dataarr = (matvar_t **) molecule_p->data;
  for(int i = 0;i < molNumber; ++i) {
    for(int j = i*fieldNumber; j < (i+1)*fieldNumber;++j) {
      string s_pd = "peakdata", s_nm = "name";
      if(dataarr[j]->name == s_pd) { //Load peakdata
	matvar_t *peakdata_p = dataarr[j];
	int numPeaks = peakdata_p->dims[0];
	molecules[i].peakdata.resize(numPeaks);
	for(int k = 0; k < numPeaks; ++k) {
	  molecules[i].peakdata[k].mass = ((double *)(peakdata_p->data))[k];
	  molecules[i].peakdata[k].area = ((double *)(peakdata_p->data))[k+numPeaks];
	}
      }
      else if(dataarr[j]->name == s_nm) {
	molecules[i].name = (char *)(dataarr[j]->data);
      }
    }
    moleculesLoaded = true;
  }
  return true;
}

 matvar_t *Matfile::getElemByName(matvar_t *parent, string name) {
   if(NULL == parent)
     return NULL;
   matvar_t **dataarr = (matvar_t **) parent->data;
   for(unsigned int i = 0; i < Mat_VarGetNumberOfFields(parent); ++i)
     if(dataarr[i]->name == name) {
       return dataarr[i];
     }
   return NULL;
 }
