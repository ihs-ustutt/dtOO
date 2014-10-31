#ifndef SPLINE3DTOBLENDEDSURFACEBUILDER_H
#define	SPLINE3DTOBLENDEDSURFACEBUILDER_H

#include "analyticGeometryBuilder.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class splineSurface3d;
  
  class spline3dToBlendedSurfaceBuilder : public analyticGeometryBuilder {
  public:
    DTCLASSNAMEMETHOD(spline3dToBlendedSurfaceBuilder);
    DTCLASSLOGMETHOD(spline3dToBlendedSurfaceBuilder);    
    spline3dToBlendedSurfaceBuilder();
    virtual ~spline3dToBlendedSurfaceBuilder();
    virtual int buildPart(void);
    virtual void getResult(vectorHandling< analyticGeometry* >* aGeo);
    virtual void final(void);    
  private:
    splineSurface3d * _splineSurface3dP;
  };
}
#endif	/* SPLINE3DTOBLENDEDSURFACEBUILDER_H */

