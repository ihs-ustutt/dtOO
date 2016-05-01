#ifndef DTOCCCURVE2D_H
#define	DTOCCCURVE2D_H

#include <logMe/dtMacros.h>
#include "dtCurve2d.h"
#include <interfaceHeaven/ptrHandling.h>

class Geom2d_Curve;

namespace dtOO {
  class dtOCCCurve2dBase; 
  
  class dtOCCCurve2d : public dtCurve2d {
  public:
    dt__class(dtOCCCurve2d, dtCurve2d);
    dtOCCCurve2d();
    dtOCCCurve2d( dtOCCCurve2dBase const & orig);
    virtual ~dtOCCCurve2d();
    //
    // overload
    //
    virtual float minPara ( int const & dir ) const;
    virtual float maxPara ( int const & dir ) const;
//    virtual int kind( void ) const;
    virtual bool closed( void ) const;
    virtual dtPoint2 point( float const uu ) const;
    virtual dtVector2 normal( float const & uu) const;
    virtual dtVector2 firstDer( float const uu) const;
    virtual float l_u( float const uu ) const;
    virtual float u_l( float const length ) const;
	  virtual float reparam(dtPoint2 const point) const;
    //
    // optional
    //   
	  virtual std::string dumpToString( void ) const;    
	  virtual void translate( dtVector2 const & tt );    
    //
    // optional
    //   
    virtual void revert( void );        
    dtOCCCurve2dBase const & OCCRef( void ) const;
  private:
    ptrHandling< dtOCCCurve2dBase > _curve;
    Geom2d_Curve const * _ptr;
  };
}
#endif	/* DTOCCCURVE2D_H */

