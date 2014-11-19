#ifndef conformalMappingScale_H
#define	conformalMappingScale_H

#include "dtTransformer.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class map3dTo3d;
  
  class conformalMappingScale : public dtTransformer {
  public:      
    dt__CLASSNAME(conformalMappingScale);
    conformalMappingScale();
    conformalMappingScale( conformalMappingScale const & orig );
    virtual ~conformalMappingScale();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    void init( 
      QDomElement const * transformerElementP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );
    virtual vectorHandling< analyticFunction * > 
    apply( vectorHandling< analyticFunction * > const * const aFP ) const;
    virtual void handleAnalyticGeometry(std::string const name, analyticGeometry const * value);
  protected:
    dt__pH(map3dTo3d const) _m3d;
  };
}

#endif	/* conformalMappingScale_H */

