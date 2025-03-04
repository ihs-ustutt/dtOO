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

#ifndef UNSTRUCTURED3DSURFACEMESH_H
#define UNSTRUCTURED3DSURFACEMESH_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/renderInterface.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class unstructured3dSurfaceMesh : public renderInterface {
public:
  dt__class(unstructured3dSurfaceMesh, renderInterface);
  unstructured3dSurfaceMesh();
  virtual ~unstructured3dSurfaceMesh();
  void addPoints(std::vector<dtPoint3> const &pp);
  void addPoint(dtPoint3 const &pp);
  void addElement(std::vector<dtInt> const &el);
  std::vector<dtPoint3> const &refP3(void) const;
  twoDArrayHandling<dtInt> const &refEl(void) const;
  dtInt getNQuads(void) const;
  dtInt getNTris(void) const;

private:
  std::vector<dtPoint3> _pp;
  twoDArrayHandling<dtInt> _el;
  dtInt _nQuads;
  dtInt _nTris;
};
} // namespace dtOO
#endif /* UNSTRUCTURED3DSURFACEMESH_H */
