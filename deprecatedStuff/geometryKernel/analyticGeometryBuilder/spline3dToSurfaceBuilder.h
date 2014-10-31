#ifndef SPLINE3DTOSURFACEBUILDER_H
#define	SPLINE3DTOSURFACEBUILDER_H

#include "analyticGeometryBuilder.h"
#include <analyticGeometryHeaven/splineSurface3d.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class spline3dToSurfaceBuilder : public analyticGeometryBuilder {
    DTCLASSNAMEMETHOD(spline3dToSurfaceBuilder);
    DTCLASSLOGMETHOD(spline3dToSurfaceBuilder);
  public:
    spline3dToSurfaceBuilder();
    virtual ~spline3dToSurfaceBuilder();
    virtual int buildPart(void);
    virtual void getResult(vectorHandling< analyticGeometry* >* aGeo);
    virtual void final(void);
  private:
    splineSurface3d * _splineSurface3dP;
  };
}
#endif	/* SPLINE3DTOSURFACEBUILDER_H */

