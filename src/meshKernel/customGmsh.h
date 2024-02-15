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

#ifndef customGmsh_H
#define	customGmsh_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
  class map1dTo3d;
  class map2dTo3d;
  class map3dTo3d;
   
  class customGmsh : public gmshBoundedVolume {
    public:
      dt__class(customGmsh, boundedVolume);
      dt__classSelfCreate(customGmsh);
      customGmsh();
      virtual ~customGmsh();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV
      );    
      dtInt handleCustomFace(
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV      
      );      
      dtInt handleCustomFace(
        map2dTo3d const * const face,
        vectorHandling< map1dTo3d * > const & oedges,
        vectorHandling< map1dTo3d * > const & iedges
      );
      dtInt handleCustomRegion(
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV      
      );
      dtInt handleAnalyticGeometry(
        ::QDomElement const & element,
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV      
      );
    private:
      static bool _registrated;
  };
}
#endif	/* customGmsh_H */
