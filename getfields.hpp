#ifndef __POLSIM__GETFIELDS_HPP_
#define __POLSIM__GETFIELDS_HPP_

#include "resonances.hpp"
#include "types.hpp"
#include "functionofpos.hpp"
#include "field.hpp"

int getfields (Field &B, unsigned int n_samp, FunctionOfPos<AccPair> &orbit, magnetvec &dipols, magnetvec &quads, magnetvec &sexts, magnetvec &vcorrs, RESONANCES &Res);

#endif

/*__POLSIM__GETFIELDS_HPP_*/
