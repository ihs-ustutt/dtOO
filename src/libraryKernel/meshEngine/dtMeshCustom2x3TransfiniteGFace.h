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

#ifndef dtMeshCustom2x3TransfiniteGFace_H
#define	dtMeshCustom2x3TransfiniteGFace_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshTransfinite2DOperator.h"

class GFace;
class MVertex;

namespace dtOO {
  class dtGmshFace;
  class scaOneD;
 
//! Hermite interpolation for mesh generation.
/*! The interpolation is done according to [1]. 
 *
 * @todo: Calculation of the derivatives should be checked if this is correct.
 *
 *  [1] https://doi.org/10.1137/0906049
 */
  class dtMeshCustom2x3TransfiniteGFace : public dtMeshTransfinite2DOperator {
    public:
      dt__class(dtMeshCustom2x3TransfiniteGFace, dtMeshOperator);     
      dt__classSelfCreate(dtMeshCustom2x3TransfiniteGFace);
      dtMeshCustom2x3TransfiniteGFace();
      dtMeshCustom2x3TransfiniteGFace(
        const dtMeshCustom2x3TransfiniteGFace& orig
      );
      virtual ~dtMeshCustom2x3TransfiniteGFace();
      virtual void jInit(
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO
      );
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO      
      );      
      void operator()( dtGmshFace * dtgr );    
    private:
      dt__pH(scaOneD) _alpha_1;
      dt__pH(scaOneD) _alpha_2;
      dt__pH(scaOneD) _beta_1;
      dt__pH(scaOneD) _beta_2;
      dt__pH(scaOneD) _beta_3;
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshCustom2x3TransfiniteGFace_H */
