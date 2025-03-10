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

#include "dtGmshVertex.h"

#include "dtGmshEdge.h"
#include "dtGmshFace.h"
#include "dtGmshModel.h"
#include <gmsh/GEdge.h>
#include <gmsh/GModel.h>
#include <gmsh/GPoint.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <progHelper.h>

#define __caCThis const_cast<dtGmshVertex *>(this)

namespace dtOO {
dtGmshVertex::dtGmshVertex(::GModel *m, dtInt tag, double ms)
  : GVertex(m, tag, ms)
{
  _dtP = dtPoint3(0., 0., 0.);
  _status = ::GEntity::MeshGenerationStatus::PENDING;
}

dtGmshVertex::dtGmshVertex(::GModel *m, dtInt tag) : GVertex(m, tag)
{
  _dtP = dtPoint3(0., 0., 0.);
  _status = ::GEntity::MeshGenerationStatus::PENDING;
}

dtGmshVertex::~dtGmshVertex() {}

GPoint dtGmshVertex::point() const
{
  return GPoint(_dtP.x(), _dtP.y(), _dtP.z(), this);
}

double dtGmshVertex::x() const { return _dtP.x(); }

double dtGmshVertex::y() const { return _dtP.y(); }

double dtGmshVertex::z() const { return _dtP.z(); }

void dtGmshVertex::setPosition(GPoint &p)
{
  _dtP = dtPoint3(p.x(), p.y(), p.z());
}

void dtGmshVertex::setPosition(dtPoint3 const p) { _dtP = p; }

void dtGmshVertex::setPosition(dtPoint3 const *const p) { _dtP = *p; }

dtPoint3 dtGmshVertex::cast2DtPoint3(void) const
{
  return dtPoint3(x(), y(), z());
}

void dtGmshVertex::addGEntity(::GEntity *const gEnt)
{
  //
  // cast
  //
  dt__ptrAss(dtGmshEdge * ge, dtGmshEdge::DownCast(gEnt));

  // add this vertex to edge
  ge->addVertex(this);
  // add edge to this vertex
  addEdge(ge);
}

bool dtGmshVertex::isEqual(
  ::GVertex const *const gv0, ::GVertex const *const gv1
)
{
  dtReal xyzRes =
    staticPropertiesHandler::getInstance()->getOptionFloat("XYZ_resolution");
  dtPoint3 v0(gv0->x(), gv0->y(), gv0->z());
  dtPoint3 v1(gv1->x(), gv1->y(), gv1->z());
  if (dtLinearAlgebra::distance(v0, v1) < xyzRes)
  {
    return true;
  }
  else
  {
    return false;
  }
}

std::list<dtGmshEdge *> dtGmshVertex::dtEdges(void) const
{
  return progHelper::vector2List(dtGmshModel::cast2DtGmshEdge(edges()));
}

std::string dtGmshVertex::getPhysicalString(void) const
{
  dt__ptrAss(
    dtGmshModel const *const gm, dtGmshModel::ConstDownCast(__caCThis->model())
  );
  return gm->getPhysicalString(this);
}
} // namespace dtOO
