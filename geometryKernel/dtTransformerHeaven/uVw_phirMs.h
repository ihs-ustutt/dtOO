#ifndef uVw_phirMs_H
#define	uVw_phirMs_H

#include "dtTransformer.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class rotatingMap2dTo3d;
  
  class uVw_phirMs : public dtTransformer {
  public:      
    dt__CLASSNAME(uVw_phirMs);
    uVw_phirMs();
    uVw_phirMs( uVw_phirMs const & orig );
    virtual ~uVw_phirMs();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    void init( 
      QDomElement const * transformerElementP, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );
    virtual vectorHandling< analyticFunction * > 
    apply( vectorHandling< analyticFunction * > const * const aFP ) const;
    virtual std::vector< dtPoint3 > 
    apply( std::vector< dtPoint3 > const * const toTrans ) const;        
    virtual void handleAnalyticGeometry(std::string const name, analyticGeometry const * value);
    virtual void handleDtVector3(std::string const name, dtVector3 const value);
  protected:
    dt__pH(rotatingMap2dTo3d const) _rM2d;
    dtVector3 _ss;
  };
}

#endif	/* uVw_phirMs_H */

