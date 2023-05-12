#ifndef ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder_H
#define	ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const;
    private: 
      static bool _registrated;          
  };
}
#endif	/* ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder_H */

