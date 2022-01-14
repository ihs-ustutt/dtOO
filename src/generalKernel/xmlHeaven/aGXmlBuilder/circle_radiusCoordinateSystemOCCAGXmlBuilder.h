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
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const;
    private: 
      static bool _registrated;            
  };
}
#endif	/* circle_radiusCoordinateSystemOCCAGXmlBuilder_H */

