#ifndef DTOCCCURVE_H
#define	DTOCCCURVE_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/ptrHandling.h>
#include "dtCurve.h"

class Geom_Curve;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCCurve : public dtCurve {
  public:
    dt__class(dtOCCCurve, dtCurve);
    dtOCCCurve();
    dtOCCCurve( dtOCCCurveBase const & orig);
    virtual ~dtOCCCurve();
    //
    // overload
    //
    virtual dtReal minPara ( int const & dir ) const;
    virtual dtReal maxPara ( int const & dir ) const;
//    virtual int getKind( void ) const;
    virtual bool closed( void ) const;
    virtual dtPoint3 point( dtReal const uu ) const;
    virtual dtVector3 firstDer( dtReal const uu) const;
    virtual dtReal l_u( dtReal const uu ) const;
    virtual dtReal u_l( dtReal const length ) const;
    virtual dtReal reparam(dtPoint3 const point) const;
    //
    // optional
    //   
	  virtual std::string dumpToString( void ) const;
    /**
     * @todo Do this in external builder.
     */
    virtual void revert(void);   
    /**
     * @todo Do this in external builder.
     */
	  virtual void translate( dtVector3 const & tt );
    dtOCCCurveBase const & OCCRef( void ) const;
    dtOCCCurveBase & OCCRef( void );
  private:
    ptrHandling< dtOCCCurveBase > _curve;
    Geom_Curve const * _ptr;
  };
}
#endif	/* DTOCCCURVE_H */

