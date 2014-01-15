/* horizontal magnetic field in spin-phaseadvance system (theta) as estimation for resonance strengths*/
/* 04.02.2013 - J. Schmidt */

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include "types.hpp"
#include "resonances.hpp"


RESONANCES::RESONANCES(double thetastep, unsigned int n_dip, unsigned int turns)
  : dtheta(thetastep), n(360/(n_dip*dtheta)), ndip(n_dip), n_turns(turns)
{
  if(thetastep == -1.0) {
    on = false;
  }
  else {
    on = true;
  }
  theta.push_back(0.0);
  kick.push_back(0.0);
  vCorrKick.push_back(0.);
  quadKick.push_back(0.);
}



// only dipoles all of same length!!! (equidistant global sampling)
void RESONANCES::addDip(const AccElement* dip)
{
  //interval_samp as approximation for sampling
  unsigned int i;
  //step in dipole
  double dkick = dip->hKick_mrad() / n; //(assume length as arclength along orbit, see AccLattice::madximport)

  //------Fehler abfangen ??--------

  for (i=0; i<n; i++) {
    theta.push_back(theta.back() + dtheta);
    kick.push_back(dkick);
    vCorrKick.push_back(0.);
    quadKick.push_back(0.);
  }

}



void RESONANCES::addOther(const AccElement* magnet, AccPair orbit)
{
  //------Fehler abfangen ??--------
  double tmpKick = magnet->hKick_mrad(orbit);
  kick.back() +=tmpKick;
  if (magnet->type == corrector)  vCorrKick.back() += tmpKick;
  else if (magnet->type == quadrupole)  quadKick.back() += tmpKick;

}



int RESONANCES::closering()
{
  if (theta.back() != theta_max()) {
    cout << "===============================================================================" << endl;
    cout << "ERROR: resonances.cpp: Phaseadvance theta no closed solution ("<<theta.back()<<"°)" << endl;
    cout << "                       resonance-strength calculation switched off!" << endl;
    cout << "===============================================================================" << endl;
    on = false;
    return 1;
  }
  else {
    kick[0] += kick.back();  //add kick(360°) to kick(0°) (or equivalent for n_turns>1)
    vCorrKick[0] += vCorrKick.back();
    quadKick[0] += quadKick.back();
    kick.erase(kick.end()-1);
    vCorrKick.erase(vCorrKick.end()-1);
    quadKick.erase(quadKick.end()-1);
    theta.erase(theta.end()-1);
  }

  return 0;
}



unsigned int RESONANCES::size() const
{
  if (theta.size() == kick.size())
    return theta.size();
  else
    return 0;
}



void RESONANCES::clear()
{
  theta.clear();
  kick.clear();
  vCorrKick.clear();
  quadKick.clear();
  theta.push_back(0.);
  kick.push_back(0.);
  vCorrKick.push_back(0.);
  quadKick.push_back(0.);
}







void RESONANCES::set(AccLattice &lattice, FunctionOfPos<AccPair> &orbit)
{
  unsigned int t;
  double pos_tot;
  const_AccIterator it;

  if (!on) return; // calculate only if Resonance calculation is switched on

  clear(); //delete data from previous time t

  for (t=1; t<=orbit.turns(); t++) {
    for(it=lattice.getItBegin(); it !=lattice.getItEnd(); ++it) {

      if (it->second->type == dipole) { // Dipole field independent of orbit (turn)
	addDip(it->second);
      }
      else {
	pos_tot = orbit.posTotal(lattice.where(it), t);
	addOther(it->second, orbit.interp(pos_tot));
      }

    }
  }

  closering();
}




Spectrum RESONANCES::getSpectrum(unsigned int fmaxrevIn, double ampcutIn) const
{
  // construct spectrum from kick-vector
  // (unit=degree, circumference=360)
  Spectrum s(this->getkickVector(), 360, this->n_turns, fmaxrevIn, ampcutIn, degree);

  return s;
}


int RESONANCES::out(const char *filename) const
{
 unsigned int i=0;
 int w=14;
 fstream file;

 file.open(filename, ios::out);
 if (!file.is_open()) {
   cout << "ERROR: Cannot open " << filename << "." << endl;
   return 1;
 }

 file <<setw(w)<< "theta [°]" <<setw(w)<<  "kick [mrad]" << endl;
 file <<setiosflags(ios::scientific)<<showpoint<<setprecision(3);
 for (i=0; i<theta.size(); i++) {
   file <<setw(w)<< theta[i] <<setw(w)<< kick[i] << endl;
 }
 file.close();
 cout << "* Wrote " << filename  << endl;
  return 0;
}


//output of vCorr & quad kicks (harmcorr)
void RESONANCES::harmcorr_out(const char *filename) const
{
  unsigned int i=0;
  int w=14;
  fstream file;
  stringstream msg;
  
  file.open(filename, ios::out);
  if (!file.is_open()) {
    msg << "ERROR: harmcorr_out: Cannot open " << filename << ".";
    throw std::runtime_error(msg.str());
  }
  
  file <<setw(w)<< "theta / °" <<setw(w)<< "corrs [mrad]" <<setw(w)<< "quads [mrad]" <<setw(w)<< "sum [mrad]" << endl;

  file <<setiosflags(ios::scientific)<<showpoint<<setprecision(3);
  // output only between dipoles => each n-th step
  for (i=0; i<theta.size(); i+=n) {
    file <<setw(w)<< theta[i] <<setw(w)<< vCorrKick[i] <<setw(w)<< quadKick[i] <<setw(w)<< vCorrKick[i]+quadKick[i] << endl;
  }
  file.close();
  cout << "* Wrote " << filename  << endl;
}
