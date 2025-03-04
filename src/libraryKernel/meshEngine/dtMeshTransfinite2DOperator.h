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

#ifndef dtMeshTransfinite2DOperator_H
#define dtMeshTransfinite2DOperator_H

#include <dtOOTypeDef.h>

#include "dtMesh2DOperator.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>

class QDomElement;

class GFace;
class MVertex;

namespace dtOO {
class baseContainer;
class constValue;
class analyticFunction;
class analyticGeometry;
class boundedVolume;
class dtGmshFace;

class dtMeshTransfinite2DOperator : public dtMesh2DOperator {
public:
  dt__class(dtMeshTransfinite2DOperator, dtMeshOperator);
  dtMeshTransfinite2DOperator();
  dtMeshTransfinite2DOperator(const dtMeshTransfinite2DOperator &orig);
  virtual ~dtMeshTransfinite2DOperator();
  virtual void operator()(dtGmshFace *dtgf) = 0;
  static twoDArrayHandling<dtPoint2> computeEdgeLoops(dtGmshFace *gf);
  static void createTransfiniteElements(dtGmshFace *const dtgf);

private:
};
dt__I_addCloneForpVHNotImpl(dtMeshTransfinite2DOperator);
} // namespace dtOO
#endif /* dtMeshTransfinite2DOperator_H */
