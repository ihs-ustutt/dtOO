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

#include "dtGmshEdge.h"

#include "dtGmshFace.h"
#include "dtGmshModel.h"
#include "dtGmshVertex.h"
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <progHelper.h>

#include <gmsh/GFace.h>
#include <gmsh/GModel.h>
#include <gmsh/GVertex.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/MLine.h>

#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#define __caCThis const_cast<dtGmshEdge *>(this)

namespace dtOO {
dtGmshEdge::dtGmshEdge(::GModel *m, dtInt tag) : GEdge(m, tag, NULL, NULL)
{
  l_vertices.remove(NULL);
}

dtGmshEdge::dtGmshEdge(::GModel *m, dtInt tag, ::GVertex *v1, ::GVertex *v2)
  : GEdge(m, tag, v1, v2)
{
  if (v1)
    l_vertices.push_back(v1);
  if (v2)
    l_vertices.push_back(v2);

  if (!analyticGeometry::inXYZTolerance(
        dtPoint3(v1->x(), v1->y(), v1->z()), dtPoint3(v2->x(), v2->y(), v2->z())
      ))
  {
    _mm.reset(new analyticCurve(dt__tmpPtr(
      dtCurve,
      trimmedCurve_twoPointsConnectConstructOCC(
        dtPoint3(v1->x(), v1->y(), v1->z()), dtPoint3(v2->x(), v2->y(), v2->z())
      )
        .result()
    )));
  }
}

dtGmshEdge::~dtGmshEdge(void) {}

Range<double> dtGmshEdge::parBounds(int i) const
{
  if (i == 0)
  {
    return Range<double>(
      static_cast<double>(_mm->getUMin()), static_cast<double>(_mm->getUMax())
    );
  }
  else
  {
    dt__throw(parBounds(), << dt__eval(i) << std::endl << "i should be 0.");
  }
}

GPoint dtGmshEdge::point(double par) const
{
  dtPoint3 pp = _mm->getPoint(static_cast<dtReal>(par));

  return GPoint(pp.x(), pp.y(), pp.z(), this, par);
}

SVector3 dtGmshEdge::firstDer(double par) const
{
  dtVector3 vv = _mm->firstDerU(static_cast<dtReal>(par));

  return SVector3(vv.x(), vv.y(), vv.z());
}

void dtGmshEdge::setMap1dTo3d(map1dTo3d const *const base)
{
  _mm.reset(base->clone());
}

void dtGmshEdge::setMap1dTo3dNoClone(map1dTo3d *base) { _mm.reset(base); }

map1dTo3d const *dtGmshEdge::getMap1dTo3d(void) const { return _mm.get(); }

void dtGmshEdge::setBeginVertex(::GVertex *gv) { GEdge::_v0 = gv; }

void dtGmshEdge::setEndVertex(::GVertex *gv) { GEdge::_v1 = gv; }

void dtGmshEdge::setNElements(dtInt const nE)
{
  this->meshAttributes.nbPointsTransfinite = nE + 1;
  dt__forAllRefAuto(dtGmshModel::cast2DtGmshFace(this->faces()), aFace)
  {
    aFace->correctIfTransfinite();
  }
}

void dtGmshEdge::meshTransfinite(dtInt const type, dtReal const coeff)
{
  this->meshAttributes.method = MESH_TRANSFINITE;
  this->meshAttributes.typeTransfinite = type;
  this->meshAttributes.coeffTransfinite = coeff;
}

void dtGmshEdge::meshTransfiniteWNElements(
  dtInt const type, dtReal const coeff, dtInt const nElements
)
{
  if (nElements != 0)
  {
    setNElements(nElements);
    meshTransfinite(type, coeff);
  }
}

void dtGmshEdge::addGEntity(::GEntity *const gEnt)
{
  //
  // cast
  //
  dtGmshVertex *gv = dtGmshVertex::DownCast(gEnt);
  dtGmshFace *gf = dtGmshFace::DownCast(gEnt);
  dt__throwIf((gv == NULL) && (gf == NULL), addGEntity());

  if (gv)
  {
    // add this edge to vertex
    gv->addEdge(this);
    // add vertex to this edge
    addVertex(gv);
  }
  else
  {
    // add this edge to face
    gf->addEdge(this, 1);
    // add face to this edge
    addFace(gf);
  }
}

void dtGmshEdge::addElement(::MElement *me)
{
  dt__ptrAss(::MLine * aLine, dynamic_cast<::MLine *>(me));
  GEdge::addLine(aLine);
}

void dtGmshEdge::addVertex(::GVertex *gv)
{
  if (std::find(l_vertices.begin(), l_vertices.end(), gv) == l_vertices.end())
  {
    l_vertices.push_back(gv);
  }
}

::SBoundingBox3d dtGmshEdge::bounds(bool fast) const
{
  std::pair<dtPoint3, dtPoint3> bb = _mm->boundingBox();
  return ::SBoundingBox3d(
    bb.first.x(),
    bb.first.y(),
    bb.first.z(),
    bb.second.x(),
    bb.second.y(),
    bb.second.z()
  );
}

std::list<dtGmshVertex *> dtGmshEdge::dtVertices(void) const
{
  return progHelper::vector2List(dtGmshModel::cast2DtGmshVertex(vertices()));
}

std::list<dtGmshFace *> dtGmshEdge::dtFaces(void) const
{
  return progHelper::vector2List(dtGmshModel::cast2DtGmshFace(faces()));
}

bool dtGmshEdge::isEqual(::GEdge const *const ge0, ::GEdge const *const ge1)
{
  std::vector<::GVertex *> VL0 = ge0->vertices();
  std::vector<::GVertex *> VL1 = ge1->vertices();

  if (VL0.size() != VL1.size())
  {
    return false;
  }

  int counter = 0;
  std::vector<::GVertex *>::iterator V0_it;
  std::vector<::GVertex *>::iterator V1_it;
  for (V0_it = VL0.begin(); V0_it != VL0.end(); ++V0_it)
  {
    for (V1_it = VL1.begin(); V1_it != VL1.end(); ++V1_it)
    {
      if (dtGmshVertex::isEqual(*V0_it, *V1_it))
      {
        counter++;
      }
    }
  }

  if (VL0.size() == counter)
  {
    if (staticPropertiesHandler::getInstance()->optionTrue("isEqualExtendCheck"
        ))
    {
      ::GPoint p0 =
        ge0->point(0.5 * (ge0->parBounds(0).low() + ge0->parBounds(0).high()));
      double iG = 0.5;
      ::GPoint p1 = ge1->closestPoint(::SPoint3(p0.x(), p0.y(), p0.z()), iG);

      dtReal const dist = dtLinearAlgebra::distance(
        dtPoint3(p0.x(), p0.y(), p0.z()), dtPoint3(p1.x(), p1.y(), p1.z())
      );
      bool differentPoint =
        dist >
        staticPropertiesHandler::getInstance()->getOptionFloat("XYZ_resolution"
        );
      dt__debug(
        isEqual(),
        << logMe::dtFormat("p0 = (%6.2f, %6.2f, %6.2f)") % p0.x() % p0.y() %
               p0.z()
        << std::endl
        << logMe::dtFormat("p1 = (%6.2f, %6.2f, %6.2f)") % p1.x() % p1.y() %
               p1.z()
        << std::endl
        << "|p0 - p1| = " << dist << std::endl
        << "p0 != p1 = " << differentPoint
      );
      if (differentPoint)
        return false;
    }
    return true;
  }
  else
  {
    return false;
  }
}

void dtGmshEdge::setGrading(dtReal const &grading, dtReal const &type)
{
  dt__debug(
    setGrading(),
    << "edge[ " << tag() << " ] : grading = " << grading << ", type = " << type
  );
  if ((grading == 0.) && (type == 0.))
    return;

  this->meshAttributes.method = MESH_TRANSFINITE;
  this->meshAttributes.coeffTransfinite = grading;
  this->meshAttributes.typeTransfinite = type;
}

std::string dtGmshEdge::getPhysicalString(void) const
{
  dt__ptrAss(
    dtGmshModel const *const gm, dtGmshModel::ConstDownCast(__caCThis->model())
  );
  return gm->getPhysicalString(this);
}
} // namespace dtOO
