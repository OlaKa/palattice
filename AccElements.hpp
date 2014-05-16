/* == AccElements Classes ===
 * Elements of an Accelerator (like magnets).
 * used by the "AccLattice" Container
 * by Jan Schmidt <schmidt@physik.uni-bonn.de>
 */


#ifndef __ACCELEMENTS_HPP_
#define __ACCELEMENTS_HPP_

#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <vector>
#include "types.hpp"

using namespace std;


enum element_type{dipole,quadrupole,corrector,sextupole,drift};
enum element_plane{H,V,L};    //horizontal,vertical,longitudinal
enum element_family{F,D};     //focus,defocus

// abstract base class
class AccElement {
protected:
  static AccPair zeroPair;
  static AccTriple zeroTriple;

  AccTriple misalign(AccTriple perfect) const; // apply misalignments (e.g. for B)

  // data entries are allowed to be public: AccLattice class uses const AccElement!
  // (everybody is allowed to modify an Element which is not mounted in a Lattice)
public:
  string name;
  double length;
  const element_type type;
  double strength;
  
  //alignment errors:
  double dpsi; //rotation around s axis in rad


  AccElement(string _name, double _length,element_type _type, double _strength=0.)
    : name(_name),length(_length),type(_type),strength(_strength),dpsi(0.) {}
  virtual ~AccElement() {};
  virtual AccElement& operator=(const AccElement* other);

  virtual AccElement* clone() const =0;

  virtual AccTriple B() const;
  virtual AccTriple B(AccPair orbit) const;
  double hKick_mrad() const {return 1000 * B().x * length;}
  double hKick_mrad(AccPair orbit) const {return 1000 * B(orbit).x * length;}

  bool nameMatch(vector<string> &nameList) const; // true if element name matches entry in List (can include 1 wildcard *)
  bool nameMatch(string &pattern) const;           // true if element name matches pattern (can include 1 wildcard *)

  string type_string() const;    // string output of element type

  string print() const;          // string output of (some) element properties
  string printHeader() const;    // string output of header-line(s) for print()

};



// drift
class Drift : public AccElement {
public:
  Drift(string _name="nothing", double _length=0.)
    : AccElement(_name,_length,drift) {}
  ~Drift() {}
  
  virtual Drift* clone() const {return new Drift(*this);}

  virtual AccTriple B() const;
};



// two magnet classes:
//  PlaneMagnet - homogeneous field in one plane e.g. dipole,corrector (occur as horizontal,vertical,longitudinal)
//  FamilyMagnet - field in multiple plane e.g. quad, sext (occur as focus and defocus)

class PlaneMagnet : public AccElement {
protected:
  const element_plane plane;

  PlaneMagnet(string _name, double _length,element_type _type, element_plane _plane, double _strength=0.)
    : AccElement(_name,_length,_type,_strength), plane(_plane) {}

};

class FamilyMagnet : public AccElement {
protected:
  const element_family family;

  FamilyMagnet(string _name, double _length,element_type _type, element_family _family, double _strength=0.)
    : AccElement(_name,_length,_type,_strength), family(_family) {}

};







//-------- definition of magnet types -----------

class Dipole : public PlaneMagnet {
public:
  Dipole(string _name, double _length, double _strength=0., element_plane _plane=V)
    : PlaneMagnet(_name,_length,dipole,_plane,_strength) {}
  ~Dipole() {}

  virtual Dipole* clone() const {return new Dipole(*this);}

  virtual AccTriple B() const;
  virtual AccTriple B(AccPair orbit) const {return B();}

};


class Corrector : public PlaneMagnet {
public:
  Corrector(string _name, double _length, double _strength=0., element_plane _plane=V)
    : PlaneMagnet(_name,_length,corrector,_plane,_strength) {}
  ~Corrector() {}

  virtual Corrector* clone() const {return new Corrector(*this);}

  virtual AccTriple B() const;
  virtual AccTriple B(AccPair orbit) const {return B();}
};


// ====== ATTENTION ====================================================================
// Change of "element_family" (F <-> D) changes sign of "B()" !
// So either set "element_family" and use absolute values for "strength"
// or always use default "element_family" (F) and set negative "strength" for D magnets.
// =====================================================================================

class Quadrupole : public FamilyMagnet {
public:
  Quadrupole(string _name, double _length, double _strength=0., element_family _family=F)
    : FamilyMagnet(_name,_length,quadrupole,_family,_strength) {}
  ~Quadrupole() {}

  virtual Quadrupole* clone() const {return new Quadrupole(*this);}

  virtual AccTriple B(AccPair orbit) const;

};


class Sextupole : public FamilyMagnet {
public:
  Sextupole(string _name, double _length, double _strength=0., element_family _family=F)
    : FamilyMagnet(_name,_length,sextupole,_family,_strength) {}
  ~Sextupole() {}

  virtual Sextupole* clone() const {return new Sextupole(*this);}

  virtual AccTriple B(AccPair orbit) const;

};



#endif
/*__ACCELEMENTS_HPP_*/