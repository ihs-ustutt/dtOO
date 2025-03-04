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

// class Ng_Mesh;
// class Ng_Result;
class GRegion;
void meshNormalsPointOutOfTheRegion(::GRegion *gr);

// Ng_Mesh *buildNetgenStructure(
//   GRegion *gr, bool importVolumeMesh, std::vector<MVertex*> &numberedV
//);
// Ng_Result Ng_OptimizeVolumeMesh( Ng_Mesh *mesh, double maxh );
// void TransferVolumeMesh(
//   GRegion *gr, Ng_Mesh *ngmesh, std::vector<MVertex*> &numberedV
//);
// void Ng_DeleteMesh (Ng_Mesh * mesh);
// void Ng_Exit();

namespace dtOO {
class dtGmshRegion;

class dtOptimizeMeshGRegion : public dtMesh3DOperator {
public:
  dt__classOnlyName(dtOptimizeMeshGRegion);
  dt__classSelfCreate(dtOptimizeMeshGRegion);
  dtOptimizeMeshGRegion();
  dtOptimizeMeshGRegion(bool gmsh, bool netgen);
  dtOptimizeMeshGRegion(const dtOptimizeMeshGRegion &orig);
  virtual ~dtOptimizeMeshGRegion();
  void operator()(dtGmshRegion *dtgr);

private:
  bool _gmsh;
  bool _netgen;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* DTOPTIMIZEMESHGREGION_H */
