#ifndef ANALYTICGEOMETRYRECONSTRUCTOR_H
#define	ANALYTICGEOMETRYRECONSTRUCTOR_H

#include "analyticGeometryFactory.h"
namespace covise {
  class coDoSet;
}
namespace dtOO {
  class analyticGeometryReconstructor : public analyticGeometryFactory {
  public:
    analyticGeometryReconstructor();
    virtual ~analyticGeometryReconstructor();
    analyticGeometry * reconFromCoDoSet( covise::coDoSet const * const obj ) const;
  private:

  };
}
#endif	/* ANALYTICGEOMETRYRECONSTRUCTOR_H */

