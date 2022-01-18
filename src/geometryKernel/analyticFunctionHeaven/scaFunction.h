#ifndef SCAFUNCTION_H
#define	SCAFUNCTION_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunction.h"

namespace dtOO {
  class scaFunction : public analyticFunction {
  public:
    dt__class(scaFunction, analyticFunction); 
    scaFunction();
    scaFunction( scaFunction const & orig);  
    virtual ~scaFunction();
    virtual dtInt yDim( void ) const;      
    virtual dtReal YFloat(aFX const & xx) const = 0;
    virtual aFY Y(aFX const & xx) const;
  };
}
#endif	/* SCAFUNCTION_H */

