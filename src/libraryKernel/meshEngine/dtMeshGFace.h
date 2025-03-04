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

#ifndef dtMeshGFace_H
#define dtMeshGFace_H

#include <dtOOTypeDef.h>

#include "dtMesh2DOperator.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtGmshFace;

class dtMeshGFace : public dtMesh2DOperator {
public:
  dt__class(dtMeshGFace, dtMeshOperator);
  dt__classSelfCreate(dtMeshGFace);
  dtMeshGFace();
  dtMeshGFace(const dtMeshGFace &orig);
  virtual ~dtMeshGFace();
  void operator()(dtGmshFace *dtgr);

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* dtMeshGFace_H */
