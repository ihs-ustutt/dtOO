#ifndef DTCURVE2D_H
#define	DTCURVE2D_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
    class dtCurve2d {
    public:
      dt__classOnlyName(dtCurve2d);
      //
      // overload
      //
      dtCurve2d();
      virtual ~dtCurve2d();
     	virtual dtCurve2d * clone( void ) const = 0;
      virtual float minPara ( int const & dir ) const = 0; 
      virtual float maxPara ( int const & dir ) const = 0;
      virtual int order( void ) const = 0;
      virtual bool closed( void ) const = 0;
      virtual dtPoint2 point( float const uu ) const = 0;
      virtual dtVector2 firstDer( float const uu) const = 0;
      virtual int nControlPoints( void ) const = 0;
      virtual dtPoint2 controlPoint( int const nPoint ) const = 0;
      virtual float l_u( float const uu ) const = 0;
      virtual float u_l( float const length ) const = 0;
	    virtual float reparam(dtPoint2 const point) const = 0;
      virtual void translate( dtVector2 const & tt ) = 0;      
      virtual dtVector2 normal( float const & uu) const = 0;      
      //
      // optional overload
      //
      virtual void dump(void);
      //
      // 
      //
      float minU ( void ) const;
      float maxU ( void ) const;      
      float u_uPercent(float const percent) const;
      float uPercent_u(float const uu) const;
      dtPoint2 pointPercent( float const percent ) const;
      dtVector2 normalPercent( float const percent) const;
      dtPoint2 lastControlPoint(void) const;      
      float length(void) const;
      float l_uPercent(float const percent) const;
      float u_lPercent( float const percent ) const;
      virtual void setControlPoint( int const nPoint, dtPoint2 const point );
    };
    
    //
    // boost requires this method in ptr_vector
    //    
    dtCurve2d * new_clone(dtCurve2d const & dtC2d);    
}
#endif	/* DTCURVE2D_H */

