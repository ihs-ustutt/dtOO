#ifndef CONFORMALMAPPINGSTARTFROMPOINT_H
#define	CONFORMALMAPPINGSTARTFROMPOINT_H

#include <logMe/dtMacros.h>

#include "conformalMapping.h"

namespace dtOO {
  class conformalMappingStartFromPoint : public conformalMapping {
  public:
    dt__CLASSNAME(conformalMappingStartFromPoint);
    conformalMappingStartFromPoint();
    conformalMappingStartFromPoint(const conformalMappingStartFromPoint& orig);
    virtual ~conformalMappingStartFromPoint();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    void init( 
      QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual std::vector< dtPoint2 * > apply( std::vector< dtPoint2 * > const * const pointVecP ) const;    
    virtual void handlePoint2d(std::string const name, dtPoint2 const value);
  private:
    dtPoint2 _basePoint;
  };
}
#endif	/* CONFORMALMAPPINGSTARTFROMPOINT_H */

