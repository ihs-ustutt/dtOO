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

#include "unstructured3dSurfaceMesh.h"
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
unstructured3dSurfaceMesh::unstructured3dSurfaceMesh()
{
  _nQuads = 0;
  _nTris = 0;
}

unstructured3dSurfaceMesh::~unstructured3dSurfaceMesh() {}

void unstructured3dSurfaceMesh::addPoints(std::vector<dtPoint3> const &pp)
{
  _pp = pp;
}

void unstructured3dSurfaceMesh::addPoint(dtPoint3 const &pp)
{
  _pp.push_back(pp);
}

void unstructured3dSurfaceMesh::addElement(std::vector<dtInt> const &el)
{
  _el.push_back(el);
  if (el.size() == 4)
  {
    _nQuads++;
  }
  else if (el.size() == 3)
  {
    _nTris++;
  }
  else
  {
    dt__throw(addElement, << dt__eval(el.size()));
  }
}

std::vector<dtPoint3> const &unstructured3dSurfaceMesh::refP3(void) const
{
  return _pp;
}

twoDArrayHandling<dtInt> const &unstructured3dSurfaceMesh::refEl(void) const
{
  return _el;
}

dtInt unstructured3dSurfaceMesh::getNQuads(void) const { return _nQuads; }

dtInt unstructured3dSurfaceMesh::getNTris(void) const { return _nTris; }
} // namespace dtOO
