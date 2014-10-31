#ifndef DTSISLCURVE2D_H
#define	DTSISLCURVE2D_H

#include <dtLinearAlgebra.h>
#include <string>
#include "dtSislBase.h"
#include <deprecatedMacro.h>
#include <logMe/dtMacros.h>
#include "dtCurve2d.h"

class SISLCurve;

namespace dtOO {
  class dtSislCurve2d : public dtSislBase,
                        public dtCurve2d {
    friend class dtSislSurf;
  public:
    dt__CLASSSTD(dtSislCurve2d, dtCurve2d);
    //
    // overloaded
    //
    dtSislCurve2d();
    dtSislCurve2d( const dtSislCurve2d& orig );
    dtSislCurve2d( dtSislCurve2d const * const orig);
    dtSislCurve2d( std::vector < dtPoint2 > const * const pointV, int const order );
    dtSislCurve2d( std::string const attribute, std::vector < dtPoint2 > const * const pointV, float const factor );
  	dtSislCurve2d * clone( void ) const;
    virtual ~dtSislCurve2d();
    virtual float minPara ( int const & dir ) const;    
    virtual float maxPara ( int const & dir ) const; 
    virtual int kind( void ) const;
    virtual int order( void ) const;
    virtual bool closed( void ) const;       
    virtual dtPoint2 point( float const uu ) const;
//    virtual dtVector2 normal( float const uu) const;
    virtual dtVector2 firstDer( float const uu) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint2 controlPoint( int const nPoint ) const;
    virtual float l_u( float const uu ) const;
    virtual float u_l( float const length ) const; 
    virtual float reparam(dtPoint2 const point) const;
//    virtual std::vector< dtPoint2 > boundingBox( void );
    //
    // optional overloaded
    //
    virtual void dump(void);
    virtual void connectArithmetic( dtCurve2d const * const toConnect );
//    virtual void closeArithmetic( void );
//    virtual void closeStraight( void );
    virtual void translate( dtVector2 const transVec );
    virtual void revert( void );
//    virtual void trim( float const uuStart, float const uuEnd);
    virtual void scale( dtPoint2 const norm );
//    virtual float getFunctionValue2d( float const xx ) const;
  private:
    SISLCurve const * getSISLCurve( void ) const; 
//    dtSislCurve2d( SISLCurve const & curve);    
//    dtSislCurve2d( dtSislBase const * base );
//    std::vector< dtPoint2 > getIntersectionPoints2d( 
//      dtPoint2 const point, 
//      dtVector2 const vector
//    ) const;    
//    float getParameterToX( float const xx ) const;
    void pointConstruct2d( std::vector < dtPoint2 > const * const pointV, int const order );
//    void sisl_newCurve(
//      int in, int ik, double *et, double *ecoef, int ikind, int idim, int icopy, 
//      int * etSize, int * rcoefSize, int * ecoefSize
//    ) const;
    dtSislCurve2d const * cast2Sisl( dtCurve2d const * curve ) const;
    dtSislCurve2d * cast2Sisl( dtCurve2d * curve ) const;    
    void setControlPoint( int const nPoint, dtPoint2 const point );
  private:
    SISLCurve * _SISLCurveP;
    mutable int _leftknot;
  };
}
#endif	/* DTSISLCURVE2D_H */

