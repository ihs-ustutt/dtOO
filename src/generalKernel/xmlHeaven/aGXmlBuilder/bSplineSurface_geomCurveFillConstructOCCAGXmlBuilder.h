#ifndef bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder_H
#define	bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const;
    private: 
      static bool _registrated;            
  };
}

#endif	/* bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder_H */