#ifndef ROTSPLINEBUILDER_H
#define	ROTSPLINEBUILDER_H

#include "analyticGeometryBuilder.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class rotatingSpline;

  class rotSplineBuilder : public analyticGeometryBuilder {
    DTCLASSNAMEMETHOD(rotSplineBuilder);
    DTCLASSLOGMETHOD(rotSplineBuilder);
  public:
    rotSplineBuilder();
    virtual ~rotSplineBuilder();
    virtual int buildPart(void);
    virtual void getResult(vectorHandling< analyticGeometry* >* aGeo);
    virtual void final(void);
  private:
    rotatingSpline * myRotBSpline;
  };
}
#endif	/* ROTSPLINEBUILDER_H */

