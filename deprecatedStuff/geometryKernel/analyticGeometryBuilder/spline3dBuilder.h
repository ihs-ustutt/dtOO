#ifndef SPLINE3DBUILDER_H
#define	SPLINE3DBUILDER_H

#include "analyticGeometryBuilder.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class splineCurve3d;

  class spline3dBuilder : public analyticGeometryBuilder {
    DTCLASSNAMEMETHOD(spline3dBuilder);
    DTCLASSLOGMETHOD(spline3dBuilder);
  public:
    spline3dBuilder();
    virtual ~spline3dBuilder();
    virtual int buildPart(void);
    virtual void getResult(vectorHandling< analyticGeometry* >* aGeo);
    virtual void final(void);
  private:
    splineCurve3d* _splineCurve;
  };
}
#endif	/* SPLINE3DBUILDER_H */

