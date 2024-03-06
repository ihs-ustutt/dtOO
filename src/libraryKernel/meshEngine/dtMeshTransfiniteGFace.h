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

#ifndef dtMeshTransfiniteGFace_H
#define	dtMeshTransfiniteGFace_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshTransfinite2DOperator.h"
#include <interfaceHeaven/twoDArrayHandling.h>

class GFace;
class MVertex;

namespace dtOO {
  class dtGmshFace;
  
  class dtMeshTransfiniteGFace : public dtMeshTransfinite2DOperator {
    public:
      dt__class(dtMeshTransfiniteGFace, dtMeshOperator);     
      dt__classSelfCreate(dtMeshTransfiniteGFace);
      dtMeshTransfiniteGFace();
      dtMeshTransfiniteGFace(
        const dtMeshTransfiniteGFace& orig
      );
      virtual ~dtMeshTransfiniteGFace();
      void init(
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
      twoDArrayHandling< dtPoint2 > correctConstV( 
        dtGmshFace const * const dtgf, twoDArrayHandling< dtPoint2 > pUV,
        dtInt const & pos, dtInt const & nSteps, 
        std::vector< double > lengths_i, double const & L_i
      ) const;
      static twoDArrayHandling< dtPoint2 > linearInterpolateU( 
        twoDArrayHandling< dtPoint2 > pUV, 
        dtInt const & vStart, 
        dtInt const & vEnd
      );
    private:
//      std::vector< dtReal > _uCorrPos;
//      dtInt _uCorrSteps;
      std::vector< dtReal > _vCorrPos;
      dtInt _vCorrSteps;
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshTransfiniteGFace_H */
