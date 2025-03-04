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

#ifndef DTMESH1DOPERATOR_H
#define DTMESH1DOPERATOR_H

#include <dtOOTypeDef.h>

#include "dtMeshOperator.h"
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
class baseContainer;
class constValue;
class analyticFunction;
class analyticGeometry;
class boundedVolume;
class dtGmshEdge;

class dtMesh1DOperator : public dtMeshOperator {
public:
  dt__class(dtMesh1DOperator, dtMeshOperator);
  dtMesh1DOperator();
  virtual ~dtMesh1DOperator();
  virtual dtMesh1DOperator *create(void) const = 0;
  virtual void jInit(
    jsonPrimitive const &jE,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtMeshOperator const *const mO
  );
  virtual void init(
    ::QDomElement const &element,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    lvH_dtMeshOperator const *const mO
  );
  virtual void operator()(dtGmshEdge *dtge) = 0;
  static void copyMesh(dtGmshEdge *from, dtGmshEdge *to);
};
dt__I_addCloneForpVHNotImpl(dtMesh1DOperator);
} // namespace dtOO
#endif /* DTMESH1DOPERATOR_H */
