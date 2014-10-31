#ifndef DTSISLCURVE_H
#define	DTSISLCURVE_H

#include <dtLinearAlgebra.h>
#include <string>
#include "dtSislBase.h"
#include "dtCurve.h"
#include <deprecatedMacro.h>
#include <logMe/dtMacros.h>
#include "dtCurve.h"

class SISLCurve;

namespace dtOO {
  class dtSislCurve : public dtSislBase,
                      public dtCurve {
    friend class dtSislSurf;
  public:
    dt__CLASSSTD(dtSislCurve, dtCurve);
    //
    // overloaded
    //
    dtSislCurve();
    dtSislCurve( const dtSislCurve& orig );
    dtSislCurve( dtSislCurve const * const orig);
  	dtSislCurve * clone( void ) const;
    virtual ~dtSislCurve();
    virtual float getMin ( int const & dir ) const;    
    virtual float getMax ( int const & dir ) const; 
    virtual int getKind( void ) const;
    virtual int getOrder( void ) const;
    virtual bool isClosed( void ) const;       
    virtual dtPoint3 getPoint3d( float const uu ) const;
    virtual dtVector3 firstDer( float const uu) const;
    virtual int getNControlPoints( void ) const;
    virtual dtPoint3 getControlPoint3d( int const nPoint ) const;
    virtual void setControlPoint3d( int const nPoint, dtPoint3 const point ); 
    virtual float l_u( float const uu ) const;
    virtual float u_l( float const length ) const; 
    virtual float reparam(dtPoint3 const point) const;
    //
    // optional overloaded
    //
    virtual void dump(void);
    virtual void connectArithmetic( dtCurve const * const toConnect );
    virtual void closeArithmetic( void );
    virtual void closeStraight( void );
    virtual void rotate( dtPoint3 const origin, dtVector3 const vector, float const angle );
    virtual void revert( void );
    virtual void trim( float const uuStart, float const uuEnd);
    //
    // builder interface
    //
    dtSislCurve( SISLCurve const & curve);    
    SISLCurve const & SISLRef( void ) const;
    SISLCurve const * getSISLCurve( void ) const;
  private:
    dtSislCurve( dtSislBase const * base );
    dtSislCurve( std::vector < dtPoint3 > const * const pointV, int const order );
    void pointConstruct3d( std::vector < dtPoint3 > const * const pointV, int const order );
    dtSislCurve const * cast2Sisl( dtCurve const * curve ) const;
    dtSislCurve * cast2Sisl( dtCurve * curve ) const;    
  private:
    SISLCurve * _SISLCurveP;
    mutable int _leftknot;
  };
}
#endif	/* DTSISLCURVE_H */

