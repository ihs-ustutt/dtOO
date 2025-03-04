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

#ifndef dtMeshGRegionWithOneLayer_H
#define dtMeshGRegionWithOneLayer_H

#include <dtOOTypeDef.h>

#include "dtMesh3DOperator.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class GEntity;
class MVertex;
class MElement;

namespace dtOO {
class dtGmshFace;
class dtGmshModel;
class dtGmshRegion;

class dtMeshGRegionWithOneLayer : public dtMesh3DOperator {
public:
  dt__classOnlyName(dtMeshGRegionWithOneLayer);
  dt__classSelfCreate(dtMeshGRegionWithOneLayer);
  dtMeshGRegionWithOneLayer();
  virtual ~dtMeshGRegionWithOneLayer();
  virtual void init(
    ::QDomElement const &element,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtMeshOperator const *const mO
  );
  virtual void operator()(dtGmshRegion *dtgr);

private:
  static bool _registrated;
};
} // namespace dtOO

#endif /* dtMeshGRegionWithOneLayer_H */
