#ifndef circle_radiusCoordinateSystemOCCAGXmlBuilder_H
#define	circle_radiusCoordinateSystemOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class circle_radiusCoordinateSystemOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(circle_radiusCoordinateSystemOCCAGXmlBuilder);
      dt__classSelfCreate(circle_radiusCoordinateSystemOCCAGXmlBuilder);
      circle_radiusCoordinateSystemOCCAGXmlBuilder();
      virtual ~circle_radiusCoordinateSystemOCCAGXmlBuilder();
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
#endif	/* circle_radiusCoordinateSystemOCCAGXmlBuilder_H */

