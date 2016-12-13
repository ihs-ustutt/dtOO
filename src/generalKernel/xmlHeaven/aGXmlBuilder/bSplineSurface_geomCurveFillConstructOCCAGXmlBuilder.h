#ifndef bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder_H
#define	bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder 
    : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder);
      bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder();
      virtual ~bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder();
      virtual std::vector< std::string > factoryAlias( void ) const;
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

#endif	/* bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder_H */