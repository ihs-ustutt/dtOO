#ifndef CURVEPACKER_H
#define	CURVEPACKER_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>

namespace dtOO {
  class dtCurve;
  
  class curvePacker {
  public:
    dt__CLASSNAME(curvePacker);
    curvePacker();
    curvePacker(int const * const intArr, double const * const doubleArr);    
//    curvePacker(const curvePacker& orig);
    virtual ~curvePacker();
    void pack(dtCurve const & cc, std::vector< int > * intArr, std::vector< double > * doubleArr) const;
    dtCurve * result( void );    
  private:
	  void sisl_newCurve(
      int in, int ik, double const * const et, double const * const ecoef, int const ikind, int const idim, int const icopy, 
      int * const etSize, int * const rcoefSize, int * const ecoefSize
    ) const;
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* CURVEPACKER_H */

