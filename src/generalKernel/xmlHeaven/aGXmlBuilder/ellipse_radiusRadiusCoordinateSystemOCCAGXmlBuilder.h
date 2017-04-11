#ifndef ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder_H
#define	ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder 
    : public aGXmlBuilder {
  public:  
    dt__classOnlyName(ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder);
    dt__classSelfCreate(ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder);
    ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder();
    virtual ~ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF,  
      aGPtrVec const * const aG,
      aGPtrVec * result 
    ) const;
  };
}
#endif	/* ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder_H */

