#ifndef NQUEEN_PROPAGATOR_H
#define NQUEEN_PROPAGATOR_H

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;
void nqueen_propagator(Space& home, IntVarArgs m);

#endif
