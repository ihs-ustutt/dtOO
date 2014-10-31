#ifndef DTCURVE2D_H
#define	DTCURVE2D_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
    class dtCurve2d {
    public:
      dt__CLASSNAME(dtCurve2d);
      //
      // overload
      //
      dtCurve2d();
      virtual ~dtCurve2d();
     	virtual dtCurve2d * clone( void ) const = 0;
      virtual float minPara ( int const & dir ) const = 0; 
      virtual float maxPara ( int const & dir ) const = 0;
//      virtual int kind( void ) const = 0;
      virtual int order( void ) const = 0;
      virtual bool closed( void ) const = 0;
      virtual dtPoint2 point( float const uu ) const = 0;
      virtual dtVector2 firstDer( float const uu) const = 0;
      virtual int nControlPoints( void ) const = 0;
      virtual dtPoint2 controlPoint( int const nPoint ) const = 0;
//      virtual void setControlPoint( int const nPoint, dtPoint2 const point ) = 0;
      virtual float l_u( float const uu ) const = 0;
      virtual float u_l( float const length ) const = 0;
//      virtual std::vector< dtPoint2 > boundingBox( void ) = 0;
	    virtual float reparam(dtPoint2 const point) const = 0;
      //
      // optional overload
      //
      virtual void dump(void);
      virtual void connectArithmetic( dtCurve2d const * const toConnect );
//      virtual void closeArithmetic( void );
//      virtual void closeStraight( void );
      virtual void translate( dtVector2 const transVec );
//      virtual void translate( dtVector3 const transVec );
//      virtual void revert( void );
//      virtual void trim( float const uuStart, float const uuEnd);
      virtual void scale( dtPoint2 const norm );
      virtual float getFunctionValue2d( float const xx ) const;
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
      dtVector2 normal( float const uu) const;
//      void trimPercent( float const uuStart, float const uuEnd);
    private:

    };
}
#endif	/* DTCURVE2D_H */

