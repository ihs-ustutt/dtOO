#ifndef geomShape_readIgesOCCAGXmlBuilder_H
#define	geomShape_readIgesOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class geomShape_readIgesOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(geomShape_readIgesOCCAGXmlBuilder);
      dt__classSelfCreate(geomShape_readIgesOCCAGXmlBuilder);
      geomShape_readIgesOCCAGXmlBuilder();
      virtual ~geomShape_readIgesOCCAGXmlBuilder();
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
#endif	/* geomShape_readIgesOCCAGXmlBuilder_H */

