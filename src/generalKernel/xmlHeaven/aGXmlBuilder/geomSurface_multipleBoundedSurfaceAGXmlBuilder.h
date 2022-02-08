#ifndef geomSurface_multipleBoundedSurfaceAGXmlBuilder_H
#define	geomSurface_multipleBoundedSurfaceAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class geomSurface_multipleBoundedSurfaceAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(geomSurface_multipleBoundedSurfaceAGXmlBuilder);
      dt__classSelfCreate(geomSurface_multipleBoundedSurfaceAGXmlBuilder);
      geomSurface_multipleBoundedSurfaceAGXmlBuilder();
      virtual ~geomSurface_multipleBoundedSurfaceAGXmlBuilder();
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
#endif	/* geomSurface_multipleBoundedSurfaceAGXmlBuilder_H */

