/*---------------------------------------------------------------------------*\\
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

\\*---------------------------------------------------------------------------*/

#ifndef DTHYBRIDOPTMESHGREGION_H
#define DTHYBRIDOPTMESHGREGION_H

#include <dtOOTypeDef.h>

#include "dtMesh3DOperator.h"
#include "meshEngine/dtGmshRegion.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <meshEngine/dtOVMMesh.h>

namespace dtOO {

class dtOVMMesh;

class dtHybridOptMeshGRegion : public dtMesh3DOperator {
public:
  dt__classOnlyName(dtHybridOptMeshGRegion);
  dt__classSelfCreate(dtHybridOptMeshGRegion);

  dtHybridOptMeshGRegion();
  dtHybridOptMeshGRegion(const dtHybridOptMeshGRegion &orig);
  virtual ~dtHybridOptMeshGRegion();

  virtual bool isStatusIndependent(void);

  void operator()(dtGmshRegion *dtgr);

private:
  struct LocalQuality {
    dtReal minimum;
    dtReal average;
  };

  bool isMovableVertex(ovmVertexH const &vH, dtOVMMesh &ovm) const;
  bool hasOptimizableElement(ovmVertexH const &vH, dtOVMMesh &ovm) const;

  LocalQuality localQuality(ovmVertexH const &vH, dtOVMMesh &ovm) const;
  bool better(LocalQuality const &candidate, LocalQuality const &current) const;

  dtPoint3 extractVertexPosition(ovmVertexH const &vH, dtOVMMesh &ovm) const;

  dtPoint3 calculateTetGoalPosition(ovmVertexH const &vH, dtOVMMesh &ovm) const;
  dtPoint3
  calculatePyramidGoalPosition(ovmVertexH const &vH, dtOVMMesh &ovm) const;
  dtPoint3 calculateGoalPosition(ovmVertexH const &vH, dtOVMMesh &ovm) const;

  std::vector<dtPoint3>
  extractCandidatePositions(ovmVertexH const &vH, dtOVMMesh &ovm) const;

  bool relocateVertex(ovmVertexH const &vH, dtOVMMesh &ovm) const;

  std::vector<ovmVertexH> extractMovableVertices(dtOVMMesh &ovm) const;
  void relocateVertices(dtOVMMesh &ovm, int nIter) const;

private:
  static bool _registrated;
};

} // namespace dtOO

#endif /* DTHYBRIDOPTMESHGREGION_H */
