/*---------------------------------------------------------------------------*\\
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

\\*---------------------------------------------------------------------------*/

#include "dtHybridOptMeshGRegion.h"

#include "dtGmshModel.h"
#include "dtMeshOperatorFactory.h"
#include "dtOVMMesh.h"
#include "meshEngine/dtGmshRegion.h"

#include <gmsh/MElement.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include <logMe/logMe.h>

namespace dtOO {

bool dtHybridOptMeshGRegion::_registrated = dtMeshOperatorFactory::registrate(
  dt__tmpPtr(dtHybridOptMeshGRegion, new dtHybridOptMeshGRegion())
);

dtHybridOptMeshGRegion::dtHybridOptMeshGRegion() : dtMesh3DOperator() {}

dtHybridOptMeshGRegion::dtHybridOptMeshGRegion(
  const dtHybridOptMeshGRegion &orig
)
  : dtMesh3DOperator(orig)
{
}

dtHybridOptMeshGRegion::~dtHybridOptMeshGRegion() {}

bool dtHybridOptMeshGRegion::isStatusIndependent(void) { return true; }

void dtHybridOptMeshGRegion::operator()(dtGmshRegion *dtgr)
{
  int const nIter = config().lookupDef<int>("_numIter", 10);

  Msg::Info(
    "dtHybridOptMeshGRegion()() : Vertex relocation, %d iterations", nIter
  );

  dtOVMMesh ovm;
  dt__forFromToIndex(0, dtgr->getNumMeshElements(), ii)
  {
    ::MElement *aMe = dtgr->getMeshElement(ii);
    ovmCellH cH = ovm.addCell(aMe);
  }

  float const logTime = FILELog::LogTime();
  if (debugTrue())
  {
    std::string fname = ::boost::str(
      ::boost::format("%s_%s_%f_0.msh") % dtgr->getPhysicalString() %
      getLabel() % logTime
    );

    dtgr->model()->writeMSH(fname, 4.0, false, true);
  }

  this->relocateVertices(ovm, nIter);

  if (debugTrue())
  {
    std::string fname = ::boost::str(
      ::boost::format("%s_%s_%f_1.msh") % dtgr->getPhysicalString() %
      getLabel() % logTime
    );

    dtgr->model()->writeMSH(fname, 4.0, false, true);
  }
}

bool dtHybridOptMeshGRegion::isMovableVertex(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  // vertex is fixed if it lies on a GFace
  if (ovm.at(vH)->onWhat()->dim() != 3)
  {
    return false;
  }
  // vertex is fixed if it belongs to a prism or hexahedron.
  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];

    if (dynamic_cast<::MPrism *>(me) || dynamic_cast<::MHexahedron *>(me))
      return false;
  }

  return true;
}

bool dtHybridOptMeshGRegion::hasOptimizableElement(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];

    if (dynamic_cast<::MTetrahedron *>(me) || dynamic_cast<::MPyramid *>(me))
      return true;
  }

  return false;
}

dtHybridOptMeshGRegion::LocalQuality
dtHybridOptMeshGRegion::localQuality(ovmVertexH const &vH, dtOVMMesh &ovm) const
{
  LocalQuality q;
  q.minimum = std::numeric_limits<dtReal>::max();
  q.average = 0.0;

  int n = 0;

  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];

    if (!dynamic_cast<::MTetrahedron *>(me) && !dynamic_cast<::MPyramid *>(me))
      continue;

    dtReal const quality = me->minSICNShapeMeasure();

    q.minimum = std::min(q.minimum, quality);
    q.average += quality;
    ++n;
  }

  if (n == 0)
  {
    q.minimum = 0.0;
    q.average = 0.0;
  }
  else
  {
    q.average /= static_cast<dtReal>(n);
  }

  return q;
}

bool dtHybridOptMeshGRegion::better(
  LocalQuality const &candidate, LocalQuality const &current
) const
{
  dtReal const eps = 1.e-8;

  if (candidate.minimum > current.minimum + eps)
    return true;

  if (std::abs(candidate.minimum - current.minimum) < eps &&
      candidate.average > current.average + eps)
    return true;

  return false;
}

dtPoint3 dtHybridOptMeshGRegion::extractVertexPosition(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  return dtPoint3(ovm[vH]->x(), ovm[vH]->y(), ovm[vH]->z());
}

dtPoint3 dtHybridOptMeshGRegion::calculateTetGoalPosition(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  // Volume-weighted barycenter of incident tetrahedra.
  dtPoint3 goal(0.0, 0.0, 0.0);
  dtReal totalWeight = 0.0;

  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];

    if (!dynamic_cast<::MTetrahedron *>(me))
      continue;

    ::SPoint3 const bary = me->barycenter();
    dtVector3 const p(bary.x(), bary.y(), bary.z());
    dtReal const weight = std::abs(me->getVolume());

    goal += weight * p;
    totalWeight += weight;
  }

  if (totalWeight > 0.0)
    goal = dtPoint3(
      goal.x() / totalWeight, goal.y() / totalWeight, goal.z() / totalWeight
    );
  else
    goal = this->extractVertexPosition(vH, ovm);

  return goal;
}

dtPoint3 dtHybridOptMeshGRegion::calculatePyramidGoalPosition(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  std::vector<dtPoint3> pp;
  int nTets = 0;
  int nPyrs = 0;
  dtVector3 nPyr;
  dtPoint3 baryPyr;
  dtReal lPyr = 0.0;

  // This is the same construction as the existing
  // dtMeshGRegion::extractPyramidGoalPosition(), but operates directly
  // on the OVM vertex/cell topology.
  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];

    if (dynamic_cast<::MTetrahedron *>(me))
    {
      ++nTets;
      ::SPoint3 const bary = me->barycenter();
      pp.push_back(dtPoint3(bary.x(), bary.y(), bary.z()));
    }
    else if (dynamic_cast<::MPyramid *>(me))
    {
      ++nPyrs;

      dtPoint3 const v0 = dtGmshModel::extractPosition(me->getVertex(0));
      dtPoint3 const v1 = dtGmshModel::extractPosition(me->getVertex(1));
      dtPoint3 const v2 = dtGmshModel::extractPosition(me->getVertex(2));
      dtPoint3 const v3 = dtGmshModel::extractPosition(me->getVertex(3));

      nPyr = dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(v1 - v0, v3 - v0)
      );

      ::SPoint3 const bary = me->barycenter();
      baryPyr = dtPoint3(bary.x(), bary.y(), bary.z());

      lPyr = std::min(
        dtLinearAlgebra::distance(v0, v3),
        std::min(
          dtLinearAlgebra::distance(v3, v2),
          std::min(
            dtLinearAlgebra::distance(v2, v1), dtLinearAlgebra::distance(v1, v0)
          )
        )
      );
    }
  }

  // The pyramid construction is unambiguous only for one incident pyramid
  // and at least one incident tetrahedron. Otherwise use the tetrahedral goal.
  if (nTets == 0 || nPyrs != 1)
    return this->calculateTetGoalPosition(vH, ovm);

  std::vector<dtReal> ss;
  ss.reserve(pp.size());

  dt__forAllRefAuto(pp, aP)
  {
    ss.push_back(dtLinearAlgebra::dotProduct(nPyr, aP - baryPyr));
  }

  dtReal const maxS = *std::max_element(ss.begin(), ss.end());
  dtReal const minS = *std::min_element(ss.begin(), ss.end());

  dtReal dir = 1.0;

  if (std::fabs(maxS) > std::fabs(minS))
  {
    if (maxS < 0.0)
      dir = -1.0;
  }
  else
  {
    if (minS < 0.0)
      dir = -1.0;
  }

  return baryPyr + lPyr / 2.0 * dir * nPyr;
}

dtPoint3 dtHybridOptMeshGRegion::calculateGoalPosition(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  int nPyramids = 0;

  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];
    if (dynamic_cast<::MPyramid *>(me))
      ++nPyramids;
  }

  if (nPyramids > 0)
    return this->calculatePyramidGoalPosition(vH, ovm);

  return this->calculateTetGoalPosition(vH, ovm);
}

std::vector<dtPoint3> dtHybridOptMeshGRegion::extractCandidatePositions(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  dtPoint3 const oldPosition = this->extractVertexPosition(vH, ovm);
  dtPoint3 const goal = this->calculateGoalPosition(vH, ovm);
  dtVector3 const direction = goal - oldPosition;

  std::vector<dtPoint3> candidates;
  candidates.push_back(oldPosition);

  // Do not jump directly to the goal. Testing intermediate positions makes
  // the relocation considerably more conservative.
  static dtReal const alpha[] = {0.05, 0.10, 0.20, 0.40, 0.60, 0.80, 1.00};

  for (dtReal const a : alpha)
    candidates.push_back(oldPosition + a * direction);

  return candidates;
}

bool dtHybridOptMeshGRegion::relocateVertex(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  if (!this->isMovableVertex(vH, ovm))
    return false;

  if (!this->hasOptimizableElement(vH, ovm))
    return false;

  dtPoint3 const oldPosition = this->extractVertexPosition(vH, ovm);
  LocalQuality const oldQuality = this->localQuality(vH, ovm);

  LocalQuality bestQuality = oldQuality;
  dtPoint3 bestPosition = oldPosition;

  std::vector<dtPoint3> const candidates =
    this->extractCandidatePositions(vH, ovm);

  for (std::size_t ii = 1; ii < candidates.size(); ++ii)
  {
    dtPoint3 const &candidate = candidates[ii];

    ovm[vH]->setXYZ(candidate.x(), candidate.y(), candidate.z());

    LocalQuality const candidateQuality = this->localQuality(vH, ovm);

    if (this->better(candidateQuality, bestQuality))
    {
      bestQuality = candidateQuality;
      bestPosition = candidate;
    }
  }

  // Always restore the best position. Thus a rejected candidate sequence
  // leaves the mesh exactly at its original position.
  ovm[vH]->setXYZ(bestPosition.x(), bestPosition.y(), bestPosition.z());

  return this->better(bestQuality, oldQuality);
}

std::vector<ovmVertexH>
dtHybridOptMeshGRegion::extractMovableVertices(dtOVMMesh &ovm) const
{
  std::vector<ovmVertexH> vertices;

  for (ovmVertexI v_it = ovm.v_iter(); v_it.valid(); ++v_it)
  {
    ovmVertexH const vH = *v_it;

    if (!this->isMovableVertex(vH, ovm))
      continue;

    if (!this->hasOptimizableElement(vH, ovm))
      continue;

    vertices.push_back(vH);
  }

  return vertices;
}

void dtHybridOptMeshGRegion::relocateVertices(dtOVMMesh &ovm, int nIter) const
{
  std::vector<ovmVertexH> const vertices = this->extractMovableVertices(ovm);

  Msg::Info(
    "dtHybridOptMeshGRegion::relocateVertices() : %d movable vertices",
    static_cast<int>(vertices.size())
  );

  for (int iter = 0; iter < nIter; ++iter)
  {
    int nRelocated = 0;

    dt__forAllRefAuto(vertices, vH)
    {
      if (this->relocateVertex(vH, ovm))
        ++nRelocated;
    }

    Msg::Info(
      "dtHybridOptMeshGRegion::relocateVertices() : iteration %d : "
      "relocated %d / %d vertices",
      iter,
      nRelocated,
      static_cast<int>(vertices.size())
    );

    if (nRelocated == 0)
    {
      Msg::Info(
        "dtHybridOptMeshGRegion::relocateVertices() : no further improvement"
      );
      break;
    }
  }
}

} // namespace dtOO
