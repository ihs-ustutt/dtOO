#ifndef THICKNESSINCREASING_H
#define	THICKNESSINCREASING_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class scaOneD;
  
  class thicknessIncreasing : public dtTransformer {
  public:    
    dt__classOnlyName(thicknessIncreasing);
    thicknessIncreasing();
    virtual ~thicknessIncreasing();
    thicknessIncreasing(const thicknessIncreasing& orig);    
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;        
    virtual bool isNecessary( void ) const;
    virtual void init( 
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );    
    virtual vectorHandling< analyticFunction * > 
    apply( vectorHandling< analyticFunction * > const * const sFunP ) const;  
    virtual void handleInt(std::string const name, int const value);
    virtual void handleAnalyticFunction(std::string const name, analyticFunction const * value);
    virtual void handleBool(std::string const name, bool const value);
  private:
    dt__pH(scaOneD const) _thicknessDistributionP;
    dt__pH(scaOneD const) _paraOnePercentFunP;
    int _nPointsOne;
    bool _isInv;
    int _splineOrder;
  };
}    

#endif	/* THICKNESSINCREASING_H */

