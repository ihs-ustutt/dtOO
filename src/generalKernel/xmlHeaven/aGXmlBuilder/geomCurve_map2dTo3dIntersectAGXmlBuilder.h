#ifndef geomCurve_map2dTo3dIntersectAGXmlBuilder_H
#define	geomCurve_map2dTo3dIntersectAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class geomCurve_map2dTo3dIntersectAGXmlBuilder : public aGXmlBuilder {
    public:
      dt__classOnlyName(geomCurve_map2dTo3dIntersectAGXmlBuilder);      
      dt__classSelfCreate(geomCurve_map2dTo3dIntersectAGXmlBuilder);      
      geomCurve_map2dTo3dIntersectAGXmlBuilder();
      virtual ~geomCurve_map2dTo3dIntersectAGXmlBuilder();
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

#endif	/* geomCurve_map2dTo3dIntersectAGXmlBuilder_H */

