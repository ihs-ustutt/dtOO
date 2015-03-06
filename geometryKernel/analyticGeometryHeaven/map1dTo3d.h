#ifndef MAP1DTO3D_H
#define	MAP1DTO3D_H

#include <dtLinearAlgebra.h>
#include "analyticGeometry.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class map1dTo3d : public analyticGeometry {
    public:
      dt__CLASSSTD(map1dTo3d, analyticGeometry);
      map1dTo3d();
      map1dTo3d(const map1dTo3d& orig);
      virtual ~map1dTo3d();
      //
      // overload
      //
      virtual map1dTo3d * create( void ) const = 0;
      virtual map1dTo3d * clone( void ) const = 0;
      virtual bool isClosed( int const & dir) const = 0;
      virtual float getMin( int const & dir) const = 0;
      virtual float getMax( int const & dir) const = 0;    
      virtual dtPoint3 getPoint( float const & uu ) const = 0;
      //
      // optional overload
      //
      virtual dtVector3 firstDerU( float const & uu) const;
      virtual float l_u( float const & uu ) const;
      virtual float u_l( float const & ll ) const;
      virtual vectorHandling< renderInterface * > getRender( void ) const;
      //
      //
      //
      dtPoint3 getPointPercent( float const & uu ) const;  
      int getRenderResolutionU( void ) const;
      float u_percent(float const & uu) const;
      float u_lPercent(float const & lP) const;
      float percent_u(float const & uu) const;
      float percent_l(float const & ll) const;      
      bool isClosedU( void ) const;
      float getUMin( void ) const;
      float getUMax( void ) const;
      float length( void ) const;
      float operator%( const float &percent ) const;        
      float operator|( const float &length ) const;  
      float operator&( const float &lengthPercent ) const;  
    private:
      float l_u( float const & uu, int const & nP ) const;
	    double funValue(const double xx ) const;
    private:
      mutable float _tmpL;
  };  
}
#endif	/* MAP1DTO3D_H */

