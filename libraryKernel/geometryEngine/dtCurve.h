#ifndef DTCURVE_H
#define	DTCURVE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
    class dtCurve {
    public:
      dt__classOnlyName(dtCurve);     
      //
      // overload
      //
      dtCurve();
      virtual ~dtCurve();
     	virtual dtCurve * clone( void ) const = 0;
      virtual float minPara ( int const & dir ) const = 0;
      virtual float maxPara ( int const & dir ) const = 0;
//      virtual int getKind( void ) const = 0;
      virtual int order( void ) const = 0;
      virtual bool closed( void ) const = 0;
      virtual dtPoint3 point( float const uu ) const = 0;
      virtual dtVector3 firstDer( float const uu) const = 0;
      virtual int nControlPoints( void ) const = 0;
      virtual dtPoint3 controlPoint( int const nPoint ) const = 0;
      virtual void setControlPoint( int const nPoint, dtPoint3 const point ) = 0;    
      virtual float l_u( float const uu ) const = 0;
      virtual float u_l( float const length ) const = 0;
      virtual float reparam(dtPoint3 const point) const = 0;
      virtual void revert( void ) = 0;
      virtual void translate( dtVector3 const & tt ) = 0;      
      //
      // optional overload
      //
      void dump(void);
      virtual std::string dumpToString(void) const;
      //
      // 
      //
      float getUMin ( void ) const;
      float getUMax ( void ) const;      
      float u_uPercent(float const percent) const;
      float uPercent_u(float const uu) const;
      dtPoint3 pointPercent( float const percent ) const;
      dtVector3 firstDerPercent( float const percent) const;    
      float length(void) const;
      float l_uPercent(float const percent) const;
      float lPercent_u(float const uu) const;
      float lPercent_uPercent(float const uuPercent) const;
      float u_lPercent( float const percent ) const;
      float uPercent_lPercent( float const percent ) const;
      float uPercent_l( float const percent ) const;      
    private:

    };
}
#endif	/* DTCURVE_H */

