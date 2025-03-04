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

#include "unstructured3dMesh.h"
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
unstructured3dMesh::unstructured3dMesh()
{
  _nTet = 0;
  _nHex = 0;
  _nPyr = 0;
  _nPri = 0;
}

unstructured3dMesh::~unstructured3dMesh() {}

void unstructured3dMesh::addPoints(std::vector<dtPoint3> const &pp)
{
  _pp = pp;
}

void unstructured3dMesh::addPoint(dtPoint3 const &pp) { _pp.push_back(pp); }

void unstructured3dMesh::addElement(std::vector<dtInt> const &el)
{
  _el.push_back(el);
  if (el.size() == 4)
  {
    _nTet++;
  }
  else if (el.size() == 5)
  {
    _nPyr++;
  }
  else if (el.size() == 6)
  {
    _nPri++;
  }
  else if (el.size() == 8)
  {
    _nHex++;
  }
}

std::vector<dtPoint3> const &unstructured3dMesh::refP3(void) const
{
  return _pp;
}

twoDArrayHandling<dtInt> const &unstructured3dMesh::refEl(void) const
{
  return _el;
}

dtInt unstructured3dMesh::getNHex(void) const { return _nHex; }

dtInt unstructured3dMesh::getNTet(void) const { return _nTet; }

dtInt unstructured3dMesh::getNPyr(void) const { return _nPyr; }

dtInt unstructured3dMesh::getNPri(void) const { return _nPri; }
} // namespace dtOO
