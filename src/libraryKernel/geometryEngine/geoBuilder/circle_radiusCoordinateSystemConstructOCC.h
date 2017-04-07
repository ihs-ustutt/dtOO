#ifndef circle_radiusCoordinateSystemConstructOCC_H
#define	circle_radiusCoordinateSystemConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class circle_radiusCoordinateSystemConstructOCC {
  public:
    dt__classOnlyName(circle_radiusCoordinateSystemConstructOCC);
    circle_radiusCoordinateSystemConstructOCC( 
      dtPoint3 const & pp, dtVector3 const & xx, float const & rr
    );
    circle_radiusCoordinateSystemConstructOCC( 
      dtPoint3 const & pp, dtVector3 const & nn, dtVector3 const & xx, 
      float const & rr
    );    
    virtual ~circle_radiusCoordinateSystemConstructOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* circle_radiusCoordinateSystemConstructOCC_H */

