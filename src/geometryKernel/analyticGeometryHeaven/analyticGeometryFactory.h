#ifndef ANALYTICGEOMETRYFACTORY_H
#define	ANALYTICGEOMETRYFACTORY_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;

  class analyticGeometryFactory {
    public:    
      dt__classOnlyName(analyticGeometryFactory);
      analyticGeometryFactory();
      virtual ~analyticGeometryFactory();
      static analyticGeometry* create(char const * const str);
  };
}
#endif	/* ANALYTICGEOMETRYFACTORY_H */

