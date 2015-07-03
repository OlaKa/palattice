/* === Particle Accelerator Lattice library (libpalattice) ===
 *
 * by Jan Schmidt <schmidt@physik.uni-bonn.de>
 *
 * This is unpublished software. Please do not copy/distribute it without
 * prior agreement of the author. Open Source publication coming soon :-)
 *
 * (c) Jan Schmidt <schmidt@physik.uni-bonn.de>, 2015
 */


#ifndef __LIBPALATTICE_HPP_
#define __LIBPALATTICE_HPP_


// namespace pal

#include "AccLattice.hpp"    // particle accelerator lattice, including madx&elegant import/export

#include "AccElements.hpp"   // particle accelerator elements (e.g. dipole,quadrupole). used by AccLattice.

#include "FunctionOfPos.hpp" // data of any type as a function of position (and turn) in a particle accelerator (e.g. orbit,trajectory,twiss). Interpolation and Spectrum (FFT) included.

#include "Spectrum.hpp"      // spectrum of any data, calculated by GSL FFT. used by FunctionOfPos.

#include "Field.hpp"         // magnetic field (3D) as a function of position (and turn). can be calculated from lattice and trajectory/orbit (FunctionOfPos).

#include "ResStrengths.hpp"  // EXPERIMENTAL - compute the complex strengths of depolarizing resonances from a lattice & orbit

#include "SimTools.hpp"      // run MadX or Elegant, read output files (SimToolTable) and manage their file names

#include "Interpolate.hpp"   // interpolateable data. wrapper for GSL interpolation. used by FunctionOfPos.

#include "Metadata.hpp"      // arbitrary meta-information (strings) as label/entry pairs with formatted output. used by AccLattice, FunctionOfPos and Spectrum.

#include "ELSASpuren.hpp"    // data structure for Closed Orbit and vert. Corrector Kicks measurement of one ELSA cycle from ELSA Control System. import included.

#include "types.hpp"         // type definitions. e.g. accelerator coordinates: 2D "Pair" (x,z) & 3D "Triple" (x,z,s)


#endif
/*__LIBPALATTICE_HPP_*/