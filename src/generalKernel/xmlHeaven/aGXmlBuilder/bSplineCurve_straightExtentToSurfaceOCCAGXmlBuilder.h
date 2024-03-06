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

#ifndef bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder_H
#define	bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder 
    : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder);
      bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder();
      virtual ~bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder_H */
