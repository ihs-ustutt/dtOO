#ifndef bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder_H
#define	bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder_H

#include <dtOOTypeDef.h>

#include <jsonHeaven/aFJsonBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder 
		: public aFJsonBuilder {
    public:
      dt__classOnlyName(bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder); 
      bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder();
      virtual ~bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder();
      virtual lvH_analyticFunction buildPart(
        jsonPrimitive const & toBuildP, 
        baseContainer const * const bC,
        lvH_constValue const * const cV, 
        lvH_analyticFunction const * const aF
      ) const;
  };
}
#endif	/* bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder_H */

