#ifndef translatingMap2dTo3d_H
#define	translatingMap2dTo3d_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include "map3dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class map2dTo3d;
  class dtTransformer;
  
  class translatingMap2dTo3d : public map3dTo3d {
  public:
    dt__class(translatingMap2dTo3d, analyticGeometry);
    translatingMap2dTo3d();
    translatingMap2dTo3d( dtVector3 const & vv, map2dTo3d const * const m2d );
    translatingMap2dTo3d(const translatingMap2dTo3d& orig);
    virtual ~translatingMap2dTo3d();
    virtual translatingMap2dTo3d * create( void ) const;        
    virtual translatingMap2dTo3d * clone( void ) const;
    virtual translatingMap2dTo3d * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const;       
    virtual bool isClosed( int const & dir) const;
    virtual dtReal getMin( int const & dir) const;
    virtual dtReal getMax( int const & dir) const;
    virtual dtPoint3 getPoint( 
      dtReal const & uu, dtReal const & vv, dtReal const & ww 
    ) const; 
	  virtual map2dTo3d * segmentConstU( dtReal const & uu ) const;
	  virtual map2dTo3d * segmentConstV( dtReal const & vv ) const;
  	virtual map2dTo3d * segmentConstW( dtReal const & ww ) const;
    virtual std::string dumpToString( void ) const;
    map2dTo3d const & constRefMap2dTo3d( void ) const;
    dtVector3 const & translationAxis( void ) const;
  private:
    ptrHandling< map2dTo3d > _m2d;
    dtVector3 _vv;
  };
}
#endif	/* translatingMap2dTo3d_H */

