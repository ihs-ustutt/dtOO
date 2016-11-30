#ifndef bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder_H
#define	bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder);
    dt__classSelfCreate(bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder);
    bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder();
    virtual ~bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuildP, 
      baseContainer * const bC,
      cVPtrVec const * const cValP, 
      aFPtrVec const * const depSFunP,
      aFPtrVec * sFunP
    ) const;
  };
}
#endif	/* bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder_H */

