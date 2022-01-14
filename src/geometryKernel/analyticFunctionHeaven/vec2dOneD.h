#ifndef VEC2DONED_H
#define	VEC2DONED_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec2dFunction.h"

namespace dtOO {
  class dtTransformer;
  
  class vec2dOneD : public vec2dFunction {
  public:
    dt__class(vec2dOneD, analyticFunction);     
    vec2dOneD();
    vec2dOneD(const vec2dOneD& orig);
    virtual ~vec2dOneD();
    virtual vec2dOneD * clone( void ) const = 0;
    virtual vec2dOneD * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const = 0;      
    virtual vec2dOneD * create( void ) const = 0;     
    virtual aFY Y( dtReal const & xx) const = 0;
    virtual aFY Y(aFX const & xx) const;
    virtual int xDim( void ) const;
    void setMin( dtReal const & min );
    void setMax( dtReal const & max );
    virtual dtReal xMin( int const & dir) const;
    virtual dtReal xMax( int const & dir) const;   
    virtual dtVector2 DYdtVector2( dtReal const & xx ) const;
    dtVector2 unitNdtVector2( dtReal const & xx ) const;
    dtVector2 unitNdtVector2Percent( dtReal const & xx ) const;
	  dtReal x_percent(dtReal const & xx) const;
    dtReal percent_x(dtReal const & xx) const;
    dtPoint2 YdtPoint2(dtReal const & xx) const;
    dtPoint2 YdtPoint2Percent(dtReal const & xx) const;
    dtReal length( dtReal const & x1 ) const;
    dtReal length( void ) const;
  private:
	  dtReal length( int const & nP, dtReal const & x1 ) const;
  private:    
    dtReal _min;
    dtReal _max;
  };
  dt__H_addCloneForpVH(vec2dOneD);        
}
#endif	/* VEC2DFUNCTION_H */

