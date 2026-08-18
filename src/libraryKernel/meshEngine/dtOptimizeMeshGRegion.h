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

#ifndef DTOPTIMIZEMESHGREGION_H
#define DTOPTIMIZEMESHGREGION_H

#include <dtOOTypeDef.h>

#include "dtMesh3DOperator.h"
#include <logMe/dtMacros.h>

class MVertex;
class MTriangle;
class MTetrahedron;
class GRegion;
void meshNormalsPointOutOfTheRegion(::GRegion *gr);

namespace dtOO {
class dtGmshRegion;

class dtOptimizeMeshGRegion : public dtMesh3DOperator {
public:
  dt__classOnlyName(dtOptimizeMeshGRegion);
  dt__classSelfCreate(dtOptimizeMeshGRegion);
  dtOptimizeMeshGRegion();
  dtOptimizeMeshGRegion(const dtOptimizeMeshGRegion &orig);
  virtual ~dtOptimizeMeshGRegion();
  virtual bool isStatusIndependent(void);
  void operator()(dtGmshRegion *dtgr);

private:
  void optimizeNetgen(dtGmshRegion *dtgr) const;
  template <typename T>
  std::vector<::MVertex *> extractVerts(std::vector<T *> elems) const;
  std::vector<::MTriangle *>
  extractSingleFaces(std::vector<::MTetrahedron *> tets) const;
  template <typename T>
  std::vector<::MTriangle *> extractCommonFaces(
    const std::vector<T *> &others, const std::vector<::MTetrahedron *> &tets
  ) const;
  void sortTetrahedraDeterministically(
    std::vector<::MTetrahedron *> &tets,
    const std::map<::MVertex *, ::MVertex *> &org_clone
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* DTOPTIMIZEMESHGREGION_H */
