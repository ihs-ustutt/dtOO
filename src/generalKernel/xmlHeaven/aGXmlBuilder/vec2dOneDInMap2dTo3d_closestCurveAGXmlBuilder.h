/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder_H
#define	vec2dOneDInMap2dTo3d_closestCurveAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
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
