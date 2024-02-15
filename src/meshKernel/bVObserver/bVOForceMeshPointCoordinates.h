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

#ifndef bVOForceMeshPointCoordinates_H
#define	bVOForceMeshPointCoordinates_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class boundedVolume;
  class vec3dThreeD;
  
  class bVOForceMeshPointCoordinates : public bVOInterface {
    public:
      dt__class(bVOForceMeshPointCoordinates, bVOInterface);
      dt__classSelfCreate(bVOForceMeshPointCoordinates);
      bVOForceMeshPointCoordinates();
      virtual ~bVOForceMeshPointCoordinates();
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
      dt__pH(vec3dThreeD) _v3d;
      std::vector< std::string > _faceLabel;
      static bool _registrated;
  };
}
#endif	/* bVOForceMeshPointCoordinates_H */
