#ifndef bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder_H
#define	bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder);
      bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder();
      virtual ~bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder();
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

#endif	/* bSplineCurve_pointPickFromPartConstructOCCAGXmlBuilder_H */

