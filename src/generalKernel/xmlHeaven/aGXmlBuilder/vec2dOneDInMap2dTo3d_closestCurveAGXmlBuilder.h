#ifndef vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder_H
#define	vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  class map1dTo3d;
  class map2dTo3d;
  
  class vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder);
      dt__classSelfCreate(vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder);
      vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder();
      virtual ~vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const;
    private:
    	double F(double const * xx) const;
    private:
      mutable dt__pH(map1dTo3d const) _m1d;
      mutable dt__pH(map2dTo3d const) _m2d;
      mutable std::vector< dtPoint3 > _sampleXYZ;
    private: 
      static bool _registrated;            
  };
}
#endif	/* vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder_H */

