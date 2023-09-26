#ifndef sca3PPointsBSplineOneD_H
#define	sca3PPointsBSplineOneD_H

#include <dtOOTypeDef.h>

#include "scaOneD.h"
#include "scaOneDPolyInterface.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunctionCompound.h"
#include "scaCurve2dOneD.h"

namespace dtOO {
  class dtTransformer;
  
  class sca3PPointsBSplineOneD : public scaOneDPolyInterface {
    public:
      dt__class(sca3PPointsBSplineOneD, analyticFunction);     
      sca3PPointsBSplineOneD();
      sca3PPointsBSplineOneD(sca3PPointsBSplineOneD const & orig);
      sca3PPointsBSplineOneD( 
        dtReal const & xB, dtReal const & yB,
        dtReal const & xA, dtReal const & yA
      );
      sca3PPointsBSplineOneD * clone( void ) const;
      sca3PPointsBSplineOneD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;        
      sca3PPointsBSplineOneD * create( void ) const;    
      virtual ~sca3PPointsBSplineOneD();
      virtual dtReal YFloat(dtReal const & xx) const;  
      virtual dtInt nDOF( void ) const;
      virtual void setDOF( std::vector< dtReal > const value );    
    private:
      scaCurve2dOneD _curve;
  };
  typedef 
    analyticFunctionCompound< sca3PPointsBSplineOneD >
    sca3PPointsBSplineOneDCompound;
}
#endif	/* sca3PPointsBSplineOneD_H */
