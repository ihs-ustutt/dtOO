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

#ifndef DTMESHGREGION_H
#define	DTMESHGREGION_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh3DOperator.h"
#include "dtOVMMesh.h"
#include "dtGmshRegion.h"

class MPyramid;

namespace dtOO {
  class dtGmshRegion;
  class dtGmshFace;
  
  class dtMeshGRegion : public dtMesh3DOperator {
    public:
      dt__class(dtMeshGRegion, dtMeshOperator);     
      dt__classSelfCreate(dtMeshGRegion);
      dtMeshGRegion();
      dtMeshGRegion(const dtMeshGRegion& orig);
      virtual ~dtMeshGRegion();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO
      );    
      void operator()( dtGmshRegion * dtgr );    
    private:
      void createPyramids( dtGmshRegion * dtgr );
      void createOVM(dtGmshRegion * dtgr, dtOVMMesh & ovm);
      static dtReal pyramidHeight( ::MPyramid * pyr );
    private:
    private:
      static bool _registrated;
  };
}
#endif	/* DTMESHGREGION_H */
