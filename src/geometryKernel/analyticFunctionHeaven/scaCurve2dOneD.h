#ifndef scaCurve2dOneD_H
#define	scaCurve2dOneD_H

#include <dtOOTypeDef.h>

#include "scaOneD.h"
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;

  class scaCurve2dOneD : public scaOneD {
    
  public:
    dt__class(scaCurve2dOneD, analyticFunction);    
    scaCurve2dOneD();
    scaCurve2dOneD(scaCurve2dOneD const & orig);
    scaCurve2dOneD( dtCurve2d const * const orig );
    scaCurve2dOneD * clone( void ) const;
	  scaCurve2dOneD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;
    scaCurve2dOneD * create( void ) const;    
    virtual ~scaCurve2dOneD();
    virtual dtReal YFloat(dtReal const & xx) const;
    dtCurve2d const * ptrDtCurve2d( void ) const;
    void translate( dtVector2 const vector );
    std::vector< dtPoint2 > getControlPoints( void ) const;
    double funValue(double const xx) const;
    double diffFunValue(double const xx) const;
    vectorHandling< renderInterface * > getRender( void ) const;  
  private:
    ptrHandling< dtCurve2d > _dtC2d;
    mutable dtReal _tmpX;
  };
}
#endif	/* scaCurve2dOneD_H */

