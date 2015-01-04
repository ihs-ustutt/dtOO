#ifndef PROJECTONSURFACE_H
#define	PROJECTONSURFACE_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticSurface;
  
  class projectOnSurface : public dtTransformer {
  public:    
    dt__CLASSNAME(projectOnSurface);
    projectOnSurface();
    virtual ~projectOnSurface();
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
    virtual vectorHandling< analyticGeometry * > apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    dt__pH(analyticSurface const) _aS;
  };
}
#endif	/* PROJECTONSURFACE_H */

