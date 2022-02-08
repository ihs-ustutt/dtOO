#ifndef ellipse_radiusRadiusCoordinateSystemConstructOCC_H
#define	ellipse_radiusRadiusCoordinateSystemConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class ellipse_radiusRadiusCoordinateSystemConstructOCC {
  public:
    dt__classOnlyName(ellipse_radiusRadiusCoordinateSystemConstructOCC);
    ellipse_radiusRadiusCoordinateSystemConstructOCC( 
      dtPoint3 const & pp, dtVector3 const & xx, 
      dtReal const & r0, dtReal const & r1
    );
    ellipse_radiusRadiusCoordinateSystemConstructOCC( 
      dtPoint3 const & pp, dtVector3 const & nn, dtVector3 const & xx, 
          dtReal const & r0, dtReal const & r1
    );    
    virtual ~ellipse_radiusRadiusCoordinateSystemConstructOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* ellipse_radiusRadiusCoordinateSystemConstructOCC_H */

