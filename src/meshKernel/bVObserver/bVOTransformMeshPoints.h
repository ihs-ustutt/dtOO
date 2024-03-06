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

#ifndef bVOTransformMeshPoints_H
#define	bVOTransformMeshPoints_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtTransformer;
  
  class bVOTransformMeshPoints : public bVOInterface {
    public:
      dt__class(bVOTransformMeshPoints, bVOInterface);
      dt__classSelfCreate(bVOTransformMeshPoints);
      bVOTransformMeshPoints();
      virtual ~bVOTransformMeshPoints();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void postUpdate( void );
    private:
      std::vector< dtTransformer const * > _dtT;
      dtReal _relTol;
      dtReal _absTol;
      bool _copy;
      static bool _registrated;
  };
}
#endif	/* bVOTransformMeshPoints_H */
