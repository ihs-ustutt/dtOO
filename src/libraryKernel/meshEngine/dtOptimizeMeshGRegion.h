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
  static bool _registrated;
};
} // namespace dtOO
#endif /* DTOPTIMIZEMESHGREGION_H */
