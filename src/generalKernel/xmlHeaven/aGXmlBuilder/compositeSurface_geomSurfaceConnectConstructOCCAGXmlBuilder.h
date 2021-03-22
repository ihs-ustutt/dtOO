#ifndef compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder_H
#define	compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder 
    : public aGXmlBuilder {
    public:  
      dt__classOnlyName(
        compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder
      );
      dt__classSelfCreate(
        compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder
      );
      compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder();
      virtual ~compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder();
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
#endif	/* compositeSurface_geomSurfaceConnectConstructOCCAGXmlBuilder_H */

