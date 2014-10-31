#ifndef DTCURVE_H
#define	DTCURVE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
    class dtCurve {
    public:
      dt__CLASSNAME(dtCurve);     
      //
      // overload
      //
      dtCurve();
      virtual ~dtCurve();
     	virtual dtCurve * clone( void ) const = 0;
      virtual float getMin ( int const & dir ) const = 0;
      virtual float getMax ( int const & dir ) const = 0;
//      virtual int getKind( void ) const = 0;
      virtual int getOrder( void ) const = 0;
      virtual bool closed( void ) const = 0;
      virtual dtPoint3 getPoint3d( float const uu ) const = 0;
      virtual dtVector3 firstDer( float const uu) const = 0;
      virtual int getNControlPoints( void ) const = 0;
      virtual dtPoint3 getControlPoint3d( int const nPoint ) const = 0;
      virtual void setControlPoint3d( int const nPoint, dtPoint3 const point ) = 0;    
      virtual float l_u( float const uu ) const = 0;
      virtual float u_l( float const length ) const = 0;
      virtual float reparam(dtPoint3 const point) const = 0;
      //
      // optional overload
      //
      virtual void dump(void);
      virtual void connectArithmetic( dtCurve const * const toConnect );
      virtual void closeArithmetic( void );
      virtual void closeStraight( void );
      virtual void rotate( dtPoint3 const origin, dtVector3 const vector, float const angle );
      virtual void revert( void );
      virtual void trim( float const uuStart, float const uuEnd);
      virtual dtPoint3 startPoint(void) const;
      virtual dtPoint3 endPoint(void) const;
      
      //
      // 
      //
      float getUMin ( void ) const;
      float getUMax ( void ) const;      
      float u_uPercent(float const percent) const;
      float uPercent_u(float const uu) const;
      dtPoint3 getPointLengthPercent3d( float const lPercent);
      dtPoint3 getPointPercent3d( float const percent ) const;
      dtVector3 firstDerPercent( float const percent) const;    
      dtPoint3 getLastControlPoint3d(void) const;
      dtPoint3 getPreLastControlPoint3d(void) const;
      dtPoint3 getFirstControlPoint3d(void) const;
      dtPoint3 getFirstNextControlPoint3d(void) const;
      float getLength(void) const;
      float l_uPercent(float const percent) const;
      float lPercent_u(float const uu) const;
      float lPercent_uPercent(float const uuPercent) const;
      float u_lPercent( float const percent ) const;
      float uPercent_lPercent( float const percent ) const;
      void trimPercent( float const uuStart, float const uuEnd);
    private:

    };
}
#endif	/* DTCURVE_H */

