#ifndef VEC3DONED_H
#define	VEC3DONED_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dFunction.h"

namespace dtOO {
  class dtTransformer;
  
  class vec3dOneD : public vec3dFunction {
  public:
    dt__class(vec3dOneD, analyticFunction);     
    vec3dOneD();
    vec3dOneD(const vec3dOneD& orig);
    virtual ~vec3dOneD();
    virtual vec3dOneD * clone( void ) const = 0;
    virtual vec3dOneD * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const = 0;      
    virtual vec3dOneD * create( void ) const = 0;    
    virtual aFY Y( dtReal const & xx) const;
    virtual aFY Y(aFX const & xx) const = 0;
    virtual int xDim( void ) const;
    void setMin( dtReal const & min );
    void setMax( dtReal const & max );
    void setMax(int const & dir, dtReal const & max);
    void setMin(int const & dir, dtReal const & min);    
    virtual dtReal xMin( int const & dir) const;
    virtual dtReal xMax( int const & dir) const;   
    virtual dtVector3 DYdtVector3( dtReal const & xx ) const;
    virtual dtVector3 DYdtVector3Percent(dtReal const & xx) const;
	  dtReal x_percent(dtReal const & xx) const;
    dtReal percent_x(dtReal const & xx) const;
    dtPoint3 YdtPoint3(dtReal const & xx) const;
    dtPoint3 YdtPoint3Percent(dtReal const & xx) const;
    dtReal length( dtReal const & x1 ) const;
    dtReal length( void ) const;
    dtReal operator%( const dtReal &percent ) const;     
  private:
	  dtReal length( int const & nP, dtReal const & x1 ) const;
  private:    
    dtReal _min;
    dtReal _max;
  };
  dt__H_addCloneForpVH(vec3dOneD);   
}
#endif	/* VEC3DONED_H */