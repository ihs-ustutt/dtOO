#ifndef SCAFUNCTION_H
#define	SCAFUNCTION_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunction.h"

namespace dtOO {
  class scaFunction : public analyticFunction {
  public:
    dt__CLASSSTD(scaFunction, analyticFunction); 
    scaFunction();
    scaFunction( scaFunction const & orig);  
    virtual ~scaFunction();
    virtual float YFloat(aFX const & xx) const = 0;
    virtual aFY Y(aFX const & xx) const;
  };
}
#endif	/* SCAFUNCTION_H */
