#include "Matfile.hpp"
#include <iostream>
#include <string>

int main(void) {
  Matfile file("test.mat");
  
  // for(int i = 0; i < 200; ++i)
  //   cout << file.massaxis(i) << ", " << file.signal(i) << endl;
  // if(file.resolutionmethod() == polynomial)
  //   cout << "Polynomial" << endl;
  // else if(file.resolutionmethod() == unknown)
  //   cout << "Unknown" << endl;
  // cout << file.resolutionparam() << endl;
  for(int i = 0; i < 100; ++i)
    cout << file.getMolecule(i).name << endl;
  return 0;
}

void showData() {
  cout << "1: Peakshape" << endl;
  cout << "2: Resolutionmethod" << endl;
  cout << "3: Resolutionparam" << endl;
  cout << "4: Resolutionlist" << endl;
  cout << "5: Comlist" << endl;
  cout << "6: Shape" << endl;
  cout << "Input: ";
  int input;
  cin >> input;
  switch(input) {
  case 1: {
    }
  default: {
  }
  }
}
