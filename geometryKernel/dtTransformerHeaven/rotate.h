#ifndef ROTATE_H
#define	ROTATE_H

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class rotate : public dtTransformer {
  public:    
    dt__CLASSNAME(rotate);
    rotate();
    virtual ~rotate();
    rotate(const rotate& orig);    
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;       
    virtual bool isNecessary( void ) const;
    void init(
      QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );
    virtual std::vector< dtPoint3 > 
    apply( std::vector< dtPoint3 > const * const toTrans ) const; 
    virtual vectorHandling< analyticGeometry * > 
    apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const;
  private:
    dtPoint3 _origin;
    dtVector3 _rotVector;
    float _angle;
  };
}
#endif	/* ROTATE_H */

