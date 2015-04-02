#ifndef EOLIB_H
#define	EOLIB_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class optimizableInterface;
  
  class eoLib {
  public:
    dt__classOnlyName(eoLib);
    eoLib();
    eoLib(const eoLib& orig);
    virtual ~eoLib();
    void optimizeInterface( optimizableInterface * const optiInt );
  private:
    // all parameters are hard-coded!
    unsigned int _SEED; // seed for random number generator
//    unsigned int _VEC_SIZE; // Number of object variables in genotypes
    unsigned int _POP_SIZE; // Size of population
    unsigned int _TOURN_SIZE; // size for tournament selection
    unsigned int _MAX_GEN; // Maximum number of generation before STOP
    float _CROSS_RATE; // Crossover rate
    double _EPSI;  // range for real uniform mutation
    float _MUT_RATE;    // mutation rate
  };
}
#endif	/* EOLIB_H */

