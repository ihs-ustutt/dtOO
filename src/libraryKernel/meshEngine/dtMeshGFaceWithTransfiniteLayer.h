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

#ifndef dtMeshGFaceWithTransfiniteLayer_H
#define dtMeshGFaceWithTransfiniteLayer_H

#include <dtOOTypeDef.h>

#include "dtMesh2DOperator.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class GFace;
class MVertex;

namespace dtOO {
class dtGmshFace;
class scaOneD;

class dtMeshGFaceWithTransfiniteLayer : public dtMesh2DOperator {
public:
  dt__class(dtMeshGFaceWithTransfiniteLayer, dtMeshOperator);
  dt__classSelfCreate(dtMeshGFaceWithTransfiniteLayer);
  dtMeshGFaceWithTransfiniteLayer();
  dtMeshGFaceWithTransfiniteLayer(const dtMeshGFaceWithTransfiniteLayer &orig);
  virtual ~dtMeshGFaceWithTransfiniteLayer();
  virtual void init(
    ::QDomElement const &element,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtMeshOperator const *const mO
  );
  void operator()(dtGmshFace *dtgr);

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* dtMeshGFaceWithTransfiniteLayer_H */
