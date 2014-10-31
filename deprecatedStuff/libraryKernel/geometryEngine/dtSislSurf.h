#ifndef DTSISLSURF_H
#define	DTSISLSURF_H

#include <dtLinearAlgebra.h>
#include "dtSislBase.h"
#include "dtSurface.h"
#include <logMe/dtMacros.h>
#include <string>
#include <vector>

class SISLSurf;

namespace dtOO {
  class dtCurve2d;
  class dtSislCurve;
  class dtSislCurve2d;
  
  class dtSislSurf : public dtSislBase,
                     public dtSurface {
  public:
    dt__CLASSSTD(dtSislSurf, dtSurface);  
    //
    // overloaded
    //
    dtSislSurf();
    virtual ~dtSislSurf();    
    virtual dtSurface * clone( void ) const;
    virtual float getMin( int const dim ) const;
    virtual float getMax( int const dim ) const;       
    virtual bool isClosed( int const dim ) const;
    virtual int getKind( void ) const;
    virtual dtPoint3 getPoint3d( float const uu, float const vv) const;
    virtual dtSislSurf * getSurface( 
      float const uuMin, float const uuMax,
      float const vvMin, float const vvMax
    ) const;
    virtual dtVector3 normal( float const uu, float const vv) const;
    virtual std::vector<dtVector3> firstDer(float const uu, float const vv) const;
	  virtual std::vector<dtVector3> secondDer(float const uu, float const vv) const;
    virtual dtPoint3 getControlPoint3d( int const uI, int const vI ) const;
    virtual void setControlPoint3d( int const uI, int const vI, dtPoint3 const point ); 
    virtual int getNControlPoints( int const dim ) const;
    virtual dtCurve * 
    getCurveLinear( 
      float const uuStart, float const vvStart,
      float const uuEnd, float const vvEnd 
    ) const;  
    virtual dtCurve * 
    getCurveConstU( 
      float const uu, float const vvMin, float const vvMax
    ) const;
    virtual dtCurve * 
    getCurveConstV( 
      float const vv, float const uuMin, float const uuMax
    ) const;    
    virtual dtCurve * getCurveSpline( dtCurve2d const * const parameterSpline ) const;  
    virtual dtPoint2 reparam(dtPoint3 const point) const;
    //
    // optional overloaded
    //
    virtual void dump( void ) const ;
    virtual void rotate( dtPoint3 const origin, dtVector3 const vector, float const angle );
    virtual void revert(void);
    virtual void offsetNormal(float const nn);
    //
    // additional
    //
    SISLSurf const * SISLPtr( void ) const;
    SISLSurf const & SISLRef( void ) const;
    dtSislSurf( SISLSurf const & surf );
  private:
    dtSislSurf( const dtSislSurf& orig );
    void updateCLength( void );
    void sisl_newSurf(
         int in1, int in2, int ik1, int ik2, double *et1, double *et2, 
         double *ecoef, int ikind, int idim, int icopy,
         int * et1Size, int * et2Size, int * rcoefSize, int * ecoefSize) const;    
    dtSislCurve const * cast2Sisl( dtCurve const * curve ) const;
    dtSislCurve * cast2Sisl( dtCurve * curve ) const;
    dtSislCurve2d const * cast2Sisl( dtCurve2d const * curve ) const;
    dtSislCurve2d * cast2Sisl( dtCurve2d * curve ) const;    
    double getUMinD ( void ) const;
    double getUMaxD ( void ) const;
    double getVMinD ( void ) const;
    double getVMaxD ( void ) const;        
    dtPoint2 getClosestPointParameterExtend(dtPoint3 const point) const;
    std::vector< dtPoint3 > getBoundingBox3d( void ) const; 
    int getDimension( void ) const;        
  private:
    SISLSurf * _SISLSurfP;
    mutable int _leftknot1;
    mutable int _leftknot2;
    float _cLengthMin;
    float _cLengthMax;
  };
}
#endif	/* DTSISLSURF_H */

