#ifndef DTOCCCURVE_H
#define	DTOCCCURVE_H

#include <logMe/dtMacros.h>
#include "dtCurve.h"
#include <interfaceHeaven/ptrHandling.h>

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
    virtual float minPara ( int const & dir ) const;
    virtual float maxPara ( int const & dir ) const;
//    virtual int getKind( void ) const;
    virtual bool closed( void ) const;
    virtual dtPoint3 point( float const uu ) const;
    virtual dtVector3 firstDer( float const uu) const;
    virtual float l_u( float const uu ) const;
    virtual float u_l( float const length ) const;
    virtual float reparam(dtPoint3 const point) const;
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
