/* === AccLattice Class ===
 * a container to store elements of an accelerator (ring) by position
 * They can be imported from MAD-X.
 * Uses the "AccElements" classes
 * by Jan Schmidt <schmidt@physik.uni-bonn.de>
 */


#ifndef __LIBPAL_ACCLATTICE_HPP_
#define __LIBPAL_ACCLATTICE_HPP_

#include <map>
#include <stdexcept>
#include <iostream>
#include <vector>
#include "AccElements.hpp"
#include "ELSASpuren.hpp"
#include "Metadata.hpp"
#include "config.hpp"
#include "types.hpp"

namespace pal
{


typedef std::map<double,AccElement*>::iterator AccIterator;
typedef std::map<double,AccElement*>::const_iterator const_AccIterator;

enum Anchor{begin,center,end};


class AccLattice {

protected:
  double circ;
  std::map<double,AccElement*> elements;                  // first: position in lattice / m
  const Drift* empty_space;
  vector<string> ignoreList;                              // elements with a name in this list (can contain 1 wildcard * per entry) are not mounted (set) in this lattice
  unsigned int ignoreCounter;

  AccIterator firstIt(element_type _type, element_plane p=noplane, element_family f=nofamily);                // get first element of given type
  AccIterator lastIt(element_type _type, element_plane p=noplane, element_family f=nofamily);                 // get last element of given type
  AccIterator nextIt(element_type _type, AccIterator it, element_plane p=noplane, element_family f=nofamily); // get next element of given type after it


public:
  const Anchor refPos;
  Metadata info;

  AccLattice(string _name, double _circumference, Anchor _refPos=begin);
  AccLattice(string _name, simulationTool s, string file, Anchor _refPos=end); //direct madx/elegant import (from file)
  AccLattice(const AccLattice &other);
  ~AccLattice();
  AccLattice& operator= (const AccLattice other);

  double circumference() const {return circ;}
  const_AccIterator getIt(double pos) const;                           // get const_Iterator to element, if pos is inside it
  const_AccIterator getItBegin() const;                                // get iterator to begin (first Element)
  const_AccIterator getItEnd() const;                                  // get iterator to end (after last Element)
  const_AccIterator firstCIt(element_type _type, element_plane p=noplane, element_family f=nofamily) const; // get iterator to first element of given type
  const_AccIterator lastCIt(element_type _type, element_plane p=noplane, element_family f=nofamily) const;  // get iterator to last element of given type
  const_AccIterator nextCIt(element_type _type, const_AccIterator it, element_plane p=noplane, element_family f=nofamily) const; // get iterator to next element of given type after it

  double where(const_AccIterator it) const {return it->first;}          // get position of element with iterator it
  double locate(double pos, const AccElement *obj, Anchor here) const;  // get here=begin/center/end (in meter) of obj at reference-position pos
  bool inside(double pos, const AccElement *obj, double here) const;    // test if "here" is inside obj at position pos
  double locate(const_AccIterator it, Anchor here) const;        // get here=begin/center/end (in meter)  of lattice element "it"
  bool inside(const_AccIterator it, double here) const;                 // test if "here" is inside lattice element "it"
  const AccElement* first(element_type _type, element_plane p=noplane, element_family f=nofamily);             // get first element of given type
  const AccElement* last(element_type _type, element_plane p=noplane, element_family f=nofamily);              // get last element of given type
  const AccElement* next(element_type _type, double pos, element_plane p=noplane, element_family f=nofamily);  // get next element of given type after pos


  const AccElement* operator[](double pos) const;                  // get element (any position, Drift returned if not inside any element)
  const_AccIterator operator[](string name) const;                 // get iterator by name (first match in lattice, Drift returned otherwise)
  void mount(double pos, const AccElement &obj, bool verbose=false); // mount an element (throws eNoFreeSpace if no free space for obj)
  void dismount(double pos);                                          // dismount element at Ref.position pos (if no element at pos: do nothing)

  void setIgnoreList(string ignoreFile);         // elements with a name in this list (can contain 1 wildcard * per entry) are not mounted in this lattice
  void madximport(string madxTwissFile);              // mount elements from MAD-X Lattice (read from twiss-output)
  void madximportMisalignments(string madxEalignFile);// set misalignments from MAD-X Lattice (read ealign-output)
                                                     // !! currently only rotation (dpsi) around beam axis (s) is implemented!
  void elegantimport(string elegantParamFile);        // mount elements from elegant Lattice (read from ascii parameter file ".param")
  void setELSAoptics(string spurenFolder);                    // change quad&sext strengths to values from "ELSA-Spuren"
  unsigned int setELSACorrectors(ELSASpuren &spuren, unsigned int t);// change corrector pos&strength to values from "ELSA-Spuren" at time t
  void subtractCorrectorStrengths(const AccLattice &other);    // subtract other corrector strengths from the ones of this lattice
  void subtractMisalignments(const AccLattice &other);         // subtract other misalignments from the ones of this lattice

  // "information"
  unsigned int size(element_type _type,element_plane p=noplane,element_family f=nofamily) const;        // returns number of elements of a type in this lattice
  unsigned int size() const {return elements.size();} // returns total number of elements

  vector<string> getIgnoreList() const {return ignoreList;}
  unsigned int ignoredElements() const {return ignoreCounter;}
  string refPos_string() const;
  string getElementDefs(simulationTool tool,element_type _type) const; // return elegant or madx compliant element definitions for given type
  string getLine(simulationTool tool) const; // return lattice in elegant or madx compliant "LINE=(..." format
  string getSequence(Anchor refer=center) const;    // return lattice in madx compliant "SEQUENCE" format


  // output (stdout or file)
  // If no filename is given, print to stdout.
  void print(const char *filename="") const;                     // print lattice.
  void print(element_type _type, const char *filename="") const; // print all elements of one type.
  void simToolExport(simulationTool tool, const char *filename="", madxLatticeType ltype=sequence) const; // print lattice readable by elegant or madx
  void latexexport(const char *filename="") const;             // print lattice readable by LaTeX (using lattice package by Jan Schmidt <schmidt@physik.uni-bonn.de>)
  // "shortcuts:"
  void elegantexport(const char *file="") const {simToolExport(elegant,file);}
  void madxexport(const char *file="",madxLatticeType t=sequence) const {simToolExport(madx,file,t);}

};


// exceptions
class eNoElement : public std::exception {
public:

  eNoElement() {}
};

class eNoFreeSpace : public std::invalid_argument {
public:
  eNoFreeSpace(string msg) : std::invalid_argument(msg) {}
};


// data format for elegant parameters file (".param")
class paramRow {
public:
  string name;
  string type;
  string param;
  double value;
  paramRow() : name(""), type(""), param(""), value(0.) {};
};


string removeQuote(string s); //remove quotation marks ("" or '') from begin&end of string

} //namespace pal

#endif
/*__LIBPAL_ACCLATTICE_HPP_*/