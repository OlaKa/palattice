/* Magnetic Field Class
 * magnetic field distribution (3D) along an accelerator ring
 * implemented as FunctionOfPos<AccTriple> with additional function(s)
 *
 * Copyright (C) 2016 Jan Felix Schmidt <janschmidt@mailbox.org>
 *   
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Field.hpp"

using namespace pal;


// set all magnetic field values from lattice and orbit
void Field::set(AccLattice &lattice, FunctionOfPos<AccPair>& orbit, unsigned int n_samples, bool edgefields)
{
  //metadata
  stringstream stmp;
  stmp << n_samples << " points per turn";
  this->info.add("Field sampling", stmp.str());
  this->info += lattice.info;
  this->info += orbit.info;

  unsigned int i, t;
  double _pos, _pos_tot;
  stringstream msg;
  AccPair otmp;
  AccTriple Btmp;
  bool noorbit = false;

 double interval_samp = this->circ / n_samples; // sampling interval of magn. field values along ring in meter

  if (this->circ != orbit.circumference()) {
    msg << "ERROR: Field::set(): Field and orbit have different circumferences ("
	 <<this->circ <<", "<<orbit.circumference()<<").";
    throw palatticeError(msg.str());
  }

   for (t=1; t<=orbit.turns(); t++) {
     for (i=0; i<n_samples; i++) {
       _pos = i*interval_samp;
       _pos_tot = orbit.posTotal(_pos, t);
       try{
       otmp = orbit.interp(_pos_tot);
       }
       catch (std::runtime_error &e) { //no orbit available: use field without orbit
	 if (!noorbit) {
	   cout << e.what() << endl;
	   noorbit = true;
	 }
	 Btmp = lattice[_pos]->B_rf(t); //field without orbit not implemented with edgefields (AccLattice::B())
	 this->FunctionOfPos<AccTriple>::set(Btmp, _pos, t);
       }

       if (edgefields)
	 Btmp = lattice.B(_pos_tot,otmp);
       else
	 Btmp = lattice[_pos]->B_rf(t,otmp);

       this->FunctionOfPos<AccTriple>::set(Btmp, _pos, t);
     }
   }
}


//compare magnet lengths in FIELDMAP with exact lengths from lattice
//to analyse influence of sampling
int Field::magnetlengths(AccLattice &lattice, const char *filename) const
{
  unsigned int w=10;
  double tmp_start, tmp_end;
  fstream file;
  AccLattice::const_type_iterator<pal::dipole> dipIt=lattice.begin<pal::dipole>();

  file.open(filename, ios::out);
  if (!file.is_open()) {
    throw palatticeFileError(filename);
  }
  file << "deviation of start/end/length of stepwise field from lattice (field - lattice)" << endl;
  file << "n_samp = " << this->samplesInTurn(1) << " sampling points per turn" << endl;
  file << "lattice metadata:" << endl;
  file << lattice.info.out("") << endl;
  file <<setw(w)<< "Name" <<setw(w)<< "start/mm" <<setw(w) << "end/mm" <<setw(w)<< "length/mm" << endl;


  for (std::map<double,AccTriple>::const_iterator it=data.begin(); it!=data.end() && turn(it->first)<2; it++) {

    if ( dipIt.at(it->first) ) {
      tmp_start = it->first;
      while ( it!=data.end()&& turn(it->first)<2 && dipIt.at(it->first) ) it++;
      tmp_end = (--it)->first;

      //write deviations from exact values to file
      file <<setw(w)<< dipIt.element()->name;
      file <<setw(w)<< (tmp_start - dipIt.begin())*1000 <<setw(w)<< (tmp_end - dipIt.end())*1000;
      file <<setw(w)<< ((tmp_end-tmp_start) - dipIt.element()->length)*1000 << endl; 
      
      ++dipIt;
      if (dipIt == lattice.end()) break;
    }
  }

  file.close();
  cout << "* Wrote " << filename  << endl;

  return 0;
}


