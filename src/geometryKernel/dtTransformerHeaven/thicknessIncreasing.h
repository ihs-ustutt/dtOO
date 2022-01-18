#ifndef THICKNESSINCREASING_H
#define	THICKNESSINCREASING_H

#include <dtOOTypeDef.h>

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
        baseContainer * const bC,
        cVPtrVec const * const cValP,
        aFPtrVec const * const sFunP,
        aGPtrVec const * const depAGeoP 
      );    
      virtual aFPtrVec 
      apply( aFPtrVec const * const sFunP ) const;  
      virtual void handleInt(std::string const name, dtInt const value);
      virtual void handleAnalyticFunction(std::string const name, analyticFunction const * value);
      virtual void handleBool(std::string const name, bool const value);
    private:
      dt__pH(scaOneD const) _thicknessDistributionP;
      dt__pH(scaOneD const) _paraOnePercentFunP;
      dtInt _nPointsOne;
      bool _isInv;
      dtInt _splineOrder;
      static bool _registrated;    
  };
}    

#endif	/* THICKNESSINCREASING_H */

