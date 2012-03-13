/* difference-mode for harmcorr analysis */
/* 13.03.2012 - J. Schmidt */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "types.hpp"
#include "constants.hpp"
#include "ELSAimport.hpp"

using namespace std;

// read and subtract reference orbit & corrector data
int difference(char *Ref_spurenFolder, char *spuren, char *Ref_spuren, unsigned int t, double corrlength, orbitvec &bpmorbit, magnetvec &vcorrs)
{

  unsigned int i;
  BPM Ref_ELSAbpms[NBPMS];
  CORR Ref_ELSAvcorrs[NVCORRS];
  orbitvec Ref_bpmorbit;
  magnetvec Ref_vcorrs;

  //read reference
  ELSAimport(Ref_ELSAbpms, Ref_ELSAvcorrs, Ref_spurenFolder); 
  ELSAimport_getbpmorbit(Ref_ELSAbpms, Ref_bpmorbit, t);
  ELSAimport_getvcorrs(Ref_ELSAvcorrs, Ref_vcorrs, corrlength, t);
  cout << "* "<<vcorrs.size()<<" correctors and "
       <<bpmorbit.size()<<" BPMs read from "<<Ref_spurenFolder << endl;

  //subtract orbit
  if (bpmorbit.size() != Ref_bpmorbit.size()) {
    cout << "ERROR: difference.cpp: Unequal number of BPMs in "<<spuren<<" and "<<Ref_spuren<< endl;
    return 1;
  }
  for (i=0; i<bpmorbit.size(); i++) {
    if (bpmorbit[i].pos==Ref_bpmorbit[i].pos) {
      bpmorbit[i].x -= Ref_bpmorbit[i].x;
      bpmorbit[i].z -= Ref_bpmorbit[i].z;
    }
    else {
      cout << "ERROR: difference.cpp: "<<i<<". BPM position is not equal in "<<spuren<<" and "<<Ref_spuren<< endl;
      return 1;
    }
  }

  //subtract corrector strengths
  if (vcorrs.size() != Ref_vcorrs.size()) {
    cout << "ERROR: difference.cpp: Unequal number of VCs in "<<spuren<<" and "<<Ref_spuren<< endl;
    return 1;
  }
  for (i=0; i<vcorrs.size(); i++) {
    if (vcorrs[i].start==Ref_vcorrs[i].start) {
      vcorrs[i].strength -= Ref_vcorrs[i].strength;
    }
    else {
      cout << "ERROR: difference.cpp: "<<i<<". VC position is not equal in "<<spuren<<" and "<<Ref_spuren<< endl;
      return 1;
    }
  }

  return 0;
}



//output file with difference-corrector data (->harmcorr) as a function of spin-phaseadvance
int harmcorr_out(magnetvec vcorrs, magnetvec dipols, char *filename)
{
  unsigned int i=0,j=0;
 int w=14;
 unsigned int nd = dipols.size();
 unsigned int nc = vcorrs.size();
 double tmp=0;
 fstream file;
 
 file.open(filename, ios::out);
 if (!file.is_open()) {
   cout << "ERROR: harmcorr_out: Cannot open " << filename << "." << endl;
   return 1;
 }
 
 file <<setw(w)<< "dipol interval" <<setw(w)<< "strength[1/m]" << endl;
 for (i=0; i<=nd; i++) {
   if (i<nd) {
     while(vcorrs[j].start < dipols[i].start && j < nc) {
       tmp += vcorrs[j].strength;
       j++;
     }
   }
   else if (i==nd) {
     while (j<nc) {
       tmp += vcorrs[j].strength;
       j++;
     }
   }
   file <<setiosflags(ios::scientific)<<showpoint<<setprecision(3);
   file <<setw(w)<< i <<setw(w)<< tmp << endl;
   tmp = 0;
 }

 file.close();
 cout << "Wrote " << filename  << endl;

 return 0;
}