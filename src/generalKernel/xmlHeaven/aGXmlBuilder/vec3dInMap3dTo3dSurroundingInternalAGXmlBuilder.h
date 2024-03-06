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

#ifndef vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder_H
#define	vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder 
    : public aGXmlBuilder {
    public:
      dt__classOnlyName(vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder);
      dt__classSelfCreate(vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder);
      vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder();
      virtual ~vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder();
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

#endif	/* vec3dInMap3dTo3dSurroundingInternalAGXmlBuilder_H */
