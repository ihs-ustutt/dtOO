#ifndef geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder_H
#define	geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder 
    : public aFXmlBuilder {
    public:
      dt__classOnlyName(
        geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder
      );    
      dt__classSelfCreate(
        geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder
      );
      geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder();
      virtual ~geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuildP,
        baseContainer * const bC,
        cVPtrVec const * const cV, 
        aFPtrVec const * const aF,
        aFPtrVec * result
      ) const;
    private: 
      static bool _registrated;      
    };
}
#endif	/* geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder_H */

