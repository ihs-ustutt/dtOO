#ifndef DTANALYSIS_H
#define	DTANALYSIS_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace ROOT {
  namespace Math {
    class Minimizer;
    class IBaseFunctionMultiDim;
  }  
}

namespace dtOO {
  typedef ROOT::Math::Minimizer dtMinimizer;
  typedef ROOT::Math::IBaseFunctionMultiDim dtMultiFunction;
  
  class dtAnalysis {
  public:
    dt__classOnlyName(dtAnalysis);    
    dtAnalysis();
    dtAnalysis(const dtAnalysis& orig);
    virtual ~dtAnalysis();
    dtMinimizer * createMinimizer( ptrHandling< dtMultiFunction > const & fun ) const;
	  dtMultiFunction * createFunction( double (*fPtr)(double const * xx ), int const & dim ) const;
  private:

  };
}
#endif	/* DTANALYSIS_H */

