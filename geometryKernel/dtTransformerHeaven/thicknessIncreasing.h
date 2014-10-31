#ifndef THICKNESSINCREASING_H
#define	THICKNESSINCREASING_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class scaOneD;
  
  class thicknessIncreasing : public dtTransformer {
  public:    
    dt__CLASSNAME(thicknessIncreasing);
    thicknessIncreasing();
    virtual ~thicknessIncreasing();
    virtual bool isNecessary( void ) const;
    virtual void init( QDomElement * transformerElementP,
                       vectorHandling< constValue * > const * const cValP,
                       vectorHandling< analyticFunction * > const * const sFunP);
    virtual vectorHandling< analyticFunction * > apply( vectorHandling< analyticFunction * > const * const sFunP ) const;  
    virtual void handleInt(std::string const name, int const value);
    virtual void handleAnalyticFunction(std::string const name, analyticFunction const * value);
    virtual void handleBool(std::string const name, bool const value);
  private:
    scaOneD const * _thicknessDistributionP;
    scaOneD const * _paraOnePercentFunP;
    int _nPointsOne;
    bool _isInv;
    int _splineOrder;
  };
}    

#endif	/* THICKNESSINCREASING_H */

