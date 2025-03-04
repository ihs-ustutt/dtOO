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

#ifndef dtMeshAndOrientGEdge_H
#define dtMeshAndOrientGEdge_H

#include <dtOOTypeDef.h>

#include "dtMeshGEdge.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtGmshEdge;

class dtMeshAndOrientGEdge : public dtMeshGEdge {
public:
  dt__class(dtMeshAndOrientGEdge, dtMeshOperator);
  dt__classSelfCreate(dtMeshAndOrientGEdge);
  dtMeshAndOrientGEdge();
  dtMeshAndOrientGEdge(const dtMeshAndOrientGEdge &orig);
  virtual ~dtMeshAndOrientGEdge();
  virtual void operator()(dtGmshEdge *dtge);

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* dtMeshAndOrientGEdge_H */
