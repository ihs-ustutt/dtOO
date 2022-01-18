#ifndef DTOCCCURVE2D_H
#define	DTOCCCURVE2D_H

#include <dtOOTypeDef.h>

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
    virtual dtReal minPara ( dtInt const & dir ) const;
    virtual dtReal maxPara ( dtInt const & dir ) const;
//    virtual dtInt kind( void ) const;
    virtual bool closed( void ) const;
    virtual dtPoint2 point( dtReal const uu ) const;
    virtual dtVector2 normal( dtReal const & uu) const;
    virtual dtVector2 firstDer( dtReal const uu) const;
    virtual dtReal l_u( dtReal const uu ) const;
    virtual dtReal u_l( dtReal const length ) const;
	  virtual dtReal reparam(dtPoint2 const point) const;
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

