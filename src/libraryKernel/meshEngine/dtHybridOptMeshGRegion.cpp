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
    dt__warnIf(aMe->setVolumePositive() == false, operator());
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

  int const nTetIter = config().lookupDef<int>("_numTetIter", 3);

  this->optimizeTetrahedra(ovm, nTetIter);
  ovm.applyTo(dtgr);

  if (debugTrue())
  {
    std::string fname = ::boost::str(
      ::boost::format("%s_%s_%f_2.msh") % dtgr->getPhysicalString() %
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
  q.minimumVolume = std::numeric_limits<dtReal>::max();
  q.minimumRelativeVolume = std::numeric_limits<dtReal>::max();
  q.nInvalid = 0;
  q.nReversed = 0;

  int n = 0;

  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];

    if (!dynamic_cast<::MTetrahedron *>(me) && !dynamic_cast<::MPyramid *>(me))
      continue;

    dtReal const quality = me->minSICNShapeMeasure();
    q.minimum = std::min(q.minimum, quality);
    q.average += quality;

    if (dynamic_cast<::MTetrahedron *>(me))
    {
      int const volumeSign = me->getVolumeSign();

      if (volumeSign <= 0)
        ++q.nInvalid;

      if (volumeSign < 0)
        ++q.nReversed;

      dtReal const volume = std::abs(static_cast<dtReal>(me->getVolume()));

      q.minimumVolume = std::min(q.minimumVolume, volume);

      // Determine the longest edge of the tetrahedron.
      dtReal maxEdgeLengthSquared = 0.0;

      for (int i = 0; i < 4; ++i)
      {
        for (int j = i + 1; j < 4; ++j)
        {
          dtPoint3 const pi = dtGmshModel::extractPosition(me->getVertex(i));

          dtPoint3 const pj = dtGmshModel::extractPosition(me->getVertex(j));

          dtVector3 const d = pj - pi;

          dtReal const edgeLengthSquared = dtLinearAlgebra::dotProduct(d, d);

          maxEdgeLengthSquared =
            std::max(maxEdgeLengthSquared, edgeLengthSquared);
        }
      }
      if (maxEdgeLengthSquared > 0.0)
      {
        dtReal const maxEdgeLength = std::sqrt(maxEdgeLengthSquared);

        dtReal const relativeVolume =
          volume / (maxEdgeLength * maxEdgeLength * maxEdgeLength);

        q.minimumRelativeVolume =
          std::min(q.minimumRelativeVolume, relativeVolume);
      }
      else
      {
        q.minimumRelativeVolume = 0.0;
      }
    }
    ++n;
  }

  if (n == 0)
  {
    q.minimum = 0.0;
    q.average = 0.0;
    q.minimumVolume = 0.0;
    q.minimumRelativeVolume = 0.0;
    q.nInvalid = 0;
    q.nReversed = 0;
  }
  else
  {
    q.average /= static_cast<dtReal>(n);
    if (q.minimumVolume == std::numeric_limits<dtReal>::max())
      q.minimumVolume = 0.0;

    if (q.minimumRelativeVolume == std::numeric_limits<dtReal>::max())
      q.minimumRelativeVolume = 0.0;
  }

  Msg::Debug(
    "Quality:\n"
    "  minimum = %e\n"
    "  average = %e\n"
    "  minimumVolume = %e\n"
    "  minimumRelativeVolume = %e\n"
    "  nInvalid = %d\n"
    "  nReversed = %d",
    q.minimum,
    q.average,
    q.minimumVolume,
    q.minimumRelativeVolume,
    q.nInvalid,
    q.nReversed
  );
  return q;
}

bool dtHybridOptMeshGRegion::better(
  LocalQuality const &candidate, LocalQuality const &current
) const
{
  dtReal const eps = 1.e-8;
  // Invalid/reversed tetrahedra have highest priority.
  if (candidate.nInvalid < current.nInvalid)
    return true;

  // Never accept a candidate that introduces invalid tetrahedra.
  if (candidate.nInvalid > current.nInvalid)
    return false;

  // Prefer candidates that remove reversed tetrahedra.
  if (candidate.nReversed < current.nReversed)
    return true;

  // Never accept a candidate that reverses additional tetrahedra.
  if (candidate.nReversed > current.nReversed)
    return false;

  // Prefer a larger minimum SICN quality.
  if (candidate.minimum > current.minimum + eps)
    return true;

  // If the minimum SICN quality is essentially equal, prefer
  // a larger average SICN quality.
  if (std::abs(candidate.minimum - current.minimum) < eps &&
      candidate.average > current.average + eps)
    return true;

  // If the SICN qualities are essentially equal, prefer a larger
  // minimum relative tetrahedron volume.
  if (std::abs(candidate.minimum - current.minimum) < eps &&
      std::abs(candidate.average - current.average) < eps &&
      candidate.minimumRelativeVolume > current.minimumRelativeVolume + eps)
    return true;

  // Finally use the absolute volume as a tie breaker.
  if (std::abs(candidate.minimum - current.minimum) < eps &&
      std::abs(candidate.average - current.average) < eps &&
      std::abs(
        candidate.minimumRelativeVolume - current.minimumRelativeVolume
      ) < eps &&
      candidate.minimumVolume > current.minimumVolume + eps)
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

bool dtHybridOptMeshGRegion::splitTetEdge(ovmCellH const &cH, dtOVMMesh &ovm)
  const
{
  ::MElement *me = ovm[cH];

  if (!dynamic_cast<::MTetrahedron *>(me))
    return false;

  //
  // Extract the four vertices of the tetrahedron.
  //
  std::vector<ovmVertexH> vertices;

  for (ovmCellVertexI v_it = ovm.cv_iter(cH); v_it.valid(); ++v_it)
  {
    vertices.push_back(*v_it);
  }

  if (vertices.size() != 4)
    return false;

  //
  // Find the longest edge.
  //
  dtReal maxLengthSquared = -1.0;

  ovmVertexH v0;
  ovmVertexH v1;

  for (int i = 0; i < 4; ++i)
  {
    for (int j = i + 1; j < 4; ++j)
    {
      dtPoint3 const p0 = this->extractVertexPosition(vertices[i], ovm);

      dtPoint3 const p1 = this->extractVertexPosition(vertices[j], ovm);

      dtVector3 const d = p1 - p0;

      dtReal const lengthSquared = dtLinearAlgebra::dotProduct(d, d);

      if (lengthSquared > maxLengthSquared)
      {
        maxLengthSquared = lengthSquared;
        v0 = vertices[i];
        v1 = vertices[j];
      }
    }
  }

  if (!v0.is_valid() || !v1.is_valid())
    return false;

  //
  // Make sure that the complete edge neighbourhood consists
  // only of tetrahedra.
  //
  for (ovmVertexCellI c_it = ovm.vc_iter(v0); c_it.valid(); ++c_it)
  {
    ovmCellH const aCH = *c_it;

    bool containsV1 = false;

    for (ovmCellVertexI v_it = ovm.cv_iter(aCH); v_it.valid(); ++v_it)
    {
      if (*v_it == v1)
      {
        containsV1 = true;
        break;
      }
    }

    if (!containsV1)
      continue;

    if (!dynamic_cast<::MTetrahedron *>(ovm[aCH]))
      return false;
  }

  //
  // Store the quality before the split.
  //
  LocalQuality const oldQuality = this->localQuality(v0, ovm);

  //
  // Do not perform a split if the current neighbourhood already
  // contains invalid tetrahedra.
  //
  if (oldQuality.nInvalid > 0 || oldQuality.nReversed > 0)
    return false;

  //
  // Try the split. The lambda is executed while the split is
  // present in the mesh. If it returns false, dtOVMMesh rolls
  // the operation back.
  //
  return ovm.trySplitEdge(v0, v1, [&](ovmVertexH const &vNew) -> bool {
    LocalQuality const newQuality = this->localQuality(vNew, ovm);

    // Never accept a split which creates invalid tetrahedra.
    if (newQuality.nInvalid > 0 || newQuality.nReversed > 0)
    {
      Msg::Info(
        "dtHybridOptMeshGRegion::splitTetEdge() : "
        "split rejected: invalid tetrahedra "
        "(invalid=%d, reversed=%d)",
        newQuality.nInvalid,
        newQuality.nReversed
      );
      return false;
    }
    // Never accept a non-positive minimum volume.
    if (newQuality.minimumVolume <= 0.0)
    {
      Msg::Info(
        "dtHybridOptMeshGRegion::splitTetEdge() : "
        "split rejected: minimum volume=%g",
        newQuality.minimumVolume
      );
      return false;
    }
    // Never accept a non-positive relative volume.
    if (newQuality.minimumRelativeVolume <= 0.0)
    {
      Msg::Info(
        "dtHybridOptMeshGRegion::splitTetEdge() : "
        "split rejected: minimum relative volume=%g",
        newQuality.minimumRelativeVolume
      );
      return false;
    }
    // Compare the complete LocalQuality.
    if (!this->better(newQuality, oldQuality))
    {
      Msg::Info(
        "dtHybridOptMeshGRegion::splitTetEdge() : "
        "split rejected: no quality improvement "
        "(min=%g -> %g, average=%g -> %g, "
        "minVolume=%g -> %g, "
        "minRelativeVolume=%g -> %g)",
        oldQuality.minimum,
        newQuality.minimum,
        oldQuality.average,
        newQuality.average,
        oldQuality.minimumVolume,
        newQuality.minimumVolume,
        oldQuality.minimumRelativeVolume,
        newQuality.minimumRelativeVolume
      );
      return false;
    }

    Msg::Info(
      "dtHybridOptMeshGRegion::splitTetEdge() : "
      "split accepted "
      "(min=%g -> %g, average=%g -> %g, "
      "minVolume=%g -> %g, "
      "minRelativeVolume=%g -> %g)",
      oldQuality.minimum,
      newQuality.minimum,
      oldQuality.average,
      newQuality.average,
      oldQuality.minimumVolume,
      newQuality.minimumVolume,
      oldQuality.minimumRelativeVolume,
      newQuality.minimumRelativeVolume
    );

    return true;
  });
}

bool dtHybridOptMeshGRegion::optimizeTetrahedra(dtOVMMesh &ovm, int nIter) const
{
  for (int iter = 0; iter < nIter; ++iter)
  {

    int nSplits = 0;
    int nRemoves = 0;

    std::vector<ovmCellH> cells;

    for (ovmCellI c_it = ovm.c_iter(); c_it.valid(); ++c_it)
    {
      ovmCellH const cH = *c_it;

      ::MElement *me = ovm[cH];

      if (!dynamic_cast<::MTetrahedron *>(me))
        continue;

      cells.push_back(cH);
    }

    for (ovmCellH const &cH : cells)
    {
      //
      // The cell may have disappeared due to a previous split.
      //
      if (!cH.is_valid())
        continue;

      ::MElement *me = ovm[cH];
      if (me == NULL)
        continue;

      if (!dynamic_cast<::MTetrahedron *>(me))
        continue;

      //
      // Check the current quality of the tet.
      //
      dtReal const quality = me->minSICNShapeMeasure();

      //
      // Only split very poor tetrahedra.
      //
      dtReal const splitThreshold =
        config().lookupDef<dtReal>("_tetSplitThreshold", 0.10);

      if (quality >= splitThreshold)
        continue;

      if (this->splitTetEdge(cH, ovm))
      {
        nSplits = nSplits + 1;

        Msg::Info(
          "dtHybridOptMeshGRegion::optimizeTetrahedra() : "
          "split tetrahedron with quality %g",
          quality
        );

        // break;
      }
      else
      {
        if (this->removeTet(cH, ovm))
        {
          Msg::Info(
            "dtHybridOptMeshGRegion::optimizeTetrahedra() : "
            "remove tetrahedron with quality %g",
            quality
          );
          nRemoves = nRemoves + 1;
          // break;
        }
        else
        {
          Msg::Info(
            "dtHybridOptMeshGRegion::optimizeTetrahedra() : "
            "Either split nor remove of tetrahedron with quality %g was "
            "possible.",
            quality
          );
        }
      }
    }

    Msg::Info(
      "dtHybridOptMeshGRegion::optimizeTetrahedra() : iteration %d / nSplits "
      "= %d / nRemoves = %d",
      iter,
      nSplits,
      nRemoves
    );

    // end optimizeTetrahedra if nothing was performed in this iteration
    if ((nSplits == 0) && (nRemoves == 0))
      break;
  }

  return true;
}

bool dtHybridOptMeshGRegion::removeTet(ovmCellH const &cH, dtOVMMesh &ovm) const
{
  ::MElement *me = ovm[cH];

  if (!dynamic_cast<::MTetrahedron *>(me))
    return false;

  //
  // Store the local vertices of the tetrahedron.
  //
  std::vector<ovmVertexH> tetVertices;

  for (ovmCellVertexI v_it = ovm.cv_iter(cH); v_it.valid(); ++v_it)
  {
    tetVertices.push_back(*v_it);
  }

  if (tetVertices.size() != 4)
    return false;

  //
  // The quality before the operation is evaluated around
  // all vertices of the two-tet configuration.
  //
  std::vector<ovmVertexH> qualityVertices = tetVertices;

  //
  // Find the neighbouring tetrahedron sharing a face.
  //
  ovmCellH neighbour;
  std::vector<ovmVertexH> shared;

  for (ovmCellI c_it = ovm.c_iter(); c_it.valid(); ++c_it)
  {
    ovmCellH const cH2 = *c_it;

    if (cH2 == cH)
      continue;

    ::MElement *me2 = ovm[cH2];

    if (!dynamic_cast<::MTetrahedron *>(me2))
      continue;

    std::vector<ovmVertexH> common;

    for (ovmVertexH const &vH : tetVertices)
    {
      for (ovmCellVertexI v_it = ovm.cv_iter(cH2); v_it.valid(); ++v_it)
      {
        if (vH == *v_it)
        {
          common.push_back(vH);
          break;
        }
      }
    }

    if (common.size() == 3)
    {
      neighbour = cH2;
      shared = common;
      break;
    }
  }

  if (!neighbour.is_valid())
    return false;

  //
  // Add the opposite vertex of the neighbouring tetrahedron.
  //
  for (ovmCellVertexI v_it = ovm.cv_iter(neighbour); v_it.valid(); ++v_it)
  {
    bool found = false;

    for (ovmVertexH const &vH : shared)
    {
      if (*v_it == vH)
      {
        found = true;
        break;
      }
    }

    if (!found)
    {
      qualityVertices.push_back(*v_it);
      break;
    }
  }

  //
  // Remove duplicate vertices.
  //
  std::sort(
    qualityVertices.begin(),
    qualityVertices.end(),
    [](ovmVertexH const &a, ovmVertexH const &b) { return a.idx() < b.idx(); }
  );

  qualityVertices.erase(
    std::unique(qualityVertices.begin(), qualityVertices.end()),
    qualityVertices.end()
  );

  //
  // Calculate the current quality.
  //
  LocalQuality oldQuality;

  oldQuality.minimum = std::numeric_limits<dtReal>::max();

  oldQuality.average = 0.0;

  oldQuality.minimumVolume = std::numeric_limits<dtReal>::max();

  oldQuality.minimumRelativeVolume = std::numeric_limits<dtReal>::max();

  oldQuality.nInvalid = 0;
  oldQuality.nReversed = 0;

  int n = 0;

  for (ovmVertexH const &vH : qualityVertices)
  {
    LocalQuality const q = this->localQuality(vH, ovm);

    oldQuality.minimum = std::min(oldQuality.minimum, q.minimum);

    oldQuality.average += q.average;

    oldQuality.minimumVolume =
      std::min(oldQuality.minimumVolume, q.minimumVolume);

    oldQuality.minimumRelativeVolume =
      std::min(oldQuality.minimumRelativeVolume, q.minimumRelativeVolume);

    oldQuality.nInvalid += q.nInvalid;
    oldQuality.nReversed += q.nReversed;

    ++n;
  }

  if (n > 0)
    oldQuality.average /= static_cast<dtReal>(n);

  //
  // Try the local 2->3 retriangulation.
  //
  return ovm.tryRemoveTet(
    cH,
    [&](std::vector<ovmVertexH> const &affectedVertices) -> bool {
      LocalQuality newQuality;

      newQuality.minimum = std::numeric_limits<dtReal>::max();

      newQuality.average = 0.0;

      newQuality.minimumVolume = std::numeric_limits<dtReal>::max();

      newQuality.minimumRelativeVolume = std::numeric_limits<dtReal>::max();

      newQuality.nInvalid = 0;
      newQuality.nReversed = 0;

      int nNew = 0;

      for (ovmVertexH const &vH : affectedVertices)
      {
        LocalQuality const q = this->localQuality(vH, ovm);

        newQuality.minimum = std::min(newQuality.minimum, q.minimum);

        newQuality.average += q.average;

        newQuality.minimumVolume =
          std::min(newQuality.minimumVolume, q.minimumVolume);

        newQuality.minimumRelativeVolume =
          std::min(newQuality.minimumRelativeVolume, q.minimumRelativeVolume);

        newQuality.nInvalid += q.nInvalid;
        newQuality.nReversed += q.nReversed;

        ++nNew;
      }

      if (nNew > 0)
        newQuality.average /= static_cast<dtReal>(nNew);

      //
      // The new configuration must not contain invalid
      // or reversed tetrahedra.
      //
      if (newQuality.nInvalid > 0 || newQuality.nReversed > 0)
      {
        Msg::Info(
          "dtHybridOptMeshGRegion::removeTet() : "
          "2->3 retriangulation rejected: "
          "invalid=%d reversed=%d",
          newQuality.nInvalid,
          newQuality.nReversed
        );

        return false;
      }

      //
      // The resulting tetrahedra must have positive volume.
      //
      if (newQuality.minimumVolume <= 0.0 ||
          newQuality.minimumRelativeVolume <= 0.0)
      {
        return false;
      }

      //
      // Finally compare the complete LocalQuality.
      //
      if (!this->better(newQuality, oldQuality))
      {
        Msg::Info(
          "dtHybridOptMeshGRegion::removeTet() : "
          "2->3 retriangulation rejected: "
          "quality did not improve "
          "(min=%g -> %g, "
          "average=%g -> %g, "
          "minVolume=%g -> %g, "
          "minRelativeVolume=%g -> %g)",
          oldQuality.minimum,
          newQuality.minimum,
          oldQuality.average,
          newQuality.average,
          oldQuality.minimumVolume,
          newQuality.minimumVolume,
          oldQuality.minimumRelativeVolume,
          newQuality.minimumRelativeVolume
        );

        return false;
      }

      Msg::Info(
        "dtHybridOptMeshGRegion::removeTet() : "
        "2->3 retriangulation accepted "
        "(min=%g -> %g, "
        "average=%g -> %g, "
        "minVolume=%g -> %g, "
        "minRelativeVolume=%g -> %g)",
        oldQuality.minimum,
        newQuality.minimum,
        oldQuality.average,
        newQuality.average,
        oldQuality.minimumVolume,
        newQuality.minimumVolume,
        oldQuality.minimumRelativeVolume,
        newQuality.minimumRelativeVolume
      );

      return true;
    }
  );
}
} // namespace dtOO
