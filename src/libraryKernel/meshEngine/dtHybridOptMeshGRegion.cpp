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

#include "dtHybridOptMeshGRegion.h"

#include "dtGmshModel.h"
#include "dtGmshRegion.h"
#include "dtMeshOperatorFactory.h"
#include "dtOVMMesh.h"

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
  // create OVM mesh structure
  dtOVMMesh ovm;
  dt__forFromToIndex(0, dtgr->getNumMeshElements(), ii)
  {
    ::MElement *aMe = dtgr->getMeshElement(ii);
    dt__warnIf(aMe->setVolumePositive() == false, operator());
    ovm.addCell(aMe);
  }

  // write debug output
  float const logTime = FILELog::LogTime();
  if (debugTrue())
  {
    std::string fname = ::boost::str(
      ::boost::format("%s_%s_%f_0.msh") % dtgr->getPhysicalString() %
      getLabel() % logTime
    );
    dtgr->model()->writeMSH(fname, 4.0, false, true);
  }

  //
  // relocate vertices
  //
  this->relocateVertices(ovm, config().lookupDef<int>("_numIter", 10));

  // write debug output
  if (debugTrue())
  {
    std::string fname = ::boost::str(
      ::boost::format("%s_%s_%f_1.msh") % dtgr->getPhysicalString() %
      getLabel() % logTime
    );
    dtgr->model()->writeMSH(fname, 4.0, false, true);
  }

  //
  // optimize tetrahedra by split and remove
  //
  this->optimizeTetrahedra(ovm, config().lookupDef<int>("_numTetIter", 3));

  // apply changed elements to dtgr
  ovm.applyTo(dtgr);

  // write debug output
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
  // Vertices on a GFace are fixed.
  if (ovm.at(vH)->onWhat()->dim() != 3)
    return false;

  // Vertices belonging to a prism or hexahedron are fixed.
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
  // create local quality struct
  LocalQuality lQ;
  lQ.minimum = std::numeric_limits<dtReal>::max();
  lQ.average = 0.0;
  lQ.minimumVolume = std::numeric_limits<dtReal>::max();
  lQ.minimumRelativeVolume = std::numeric_limits<dtReal>::max();
  lQ.nInvalid = 0;
  lQ.nReversed = 0;

  int nElements = 0;
  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];
    ::MTetrahedron *tet = dynamic_cast<::MTetrahedron *>(me);
    ::MPyramid *pyr = dynamic_cast<::MPyramid *>(me);

    // quality is only calculated in tetrahedra and pyramids
    if (!tet && !pyr)
      continue;

    // get SICN quality of element
    dtReal const quality = me->minSICNShapeMeasure();
    lQ.minimum = std::min(lQ.minimum, quality);
    lQ.average += quality;

    // handle tetrahedra
    if (tet)
    {
      //
      // check if element is inverted or invalid
      //
      dtReal const volume = static_cast<dtReal>(me->getVolume());
      // todo: add epsilon to prevent checking against zero
      if (volume <= 0)
        ++lQ.nInvalid;
      if (volume < 0)
        ++lQ.nReversed;
      lQ.minimumVolume = std::min(lQ.minimumVolume, std::abs(volume));

      //
      // calculate squared edge lengths and store maximum
      //
      dtReal maxEdgeLengthSquared = 0.0;
      for (int i = 0; i < 4; ++i)
      {
        dtPoint3 const p_i = dtGmshModel::extractPosition(me->getVertex(i));
        for (int j = i + 1; j < 4; ++j)
        {
          dtPoint3 const p_j = dtGmshModel::extractPosition(me->getVertex(j));
          dtReal const lengthSquared = (p_j - p_i).squared_length();
          maxEdgeLengthSquared = std::max(maxEdgeLengthSquared, lengthSquared);
        }
      }

      //
      // calculate minimum relative volume
      //
      if (maxEdgeLengthSquared > 0.0)
      {
        dtReal const maxEdgeLength = std::sqrt(maxEdgeLengthSquared);
        dtReal const relativeVolume =
          volume / (maxEdgeLength * maxEdgeLength * maxEdgeLength);
        lQ.minimumRelativeVolume =
          std::min(lQ.minimumRelativeVolume, relativeVolume);
      }
      else
      {
        lQ.minimumRelativeVolume = 0.0;
      }
    }

    nElements = nElements + 1;
  }

  // calculate average
  if (nElements != 0)
  {
    lQ.average = lQ.average / static_cast<dtReal>(nElements);
    if (lQ.minimumVolume == std::numeric_limits<dtReal>::max())
      lQ.minimumVolume = 0.0;
    if (lQ.minimumRelativeVolume == std::numeric_limits<dtReal>::max())
      lQ.minimumRelativeVolume = 0.0;
  }
  // set to zero if vertex has no neighbour elements for quality calculation
  else
  {
    dt__warning(localQuality(), << "Vertex has no neighbourhood.");
    lQ.minimum = 0.0;
    lQ.average = 0.0;
    lQ.minimumVolume = 0.0;
    lQ.minimumRelativeVolume = 0.0;
    lQ.nInvalid = 0;
    lQ.nReversed = 0;
  }

  return lQ;
}

dtHybridOptMeshGRegion::LocalQuality dtHybridOptMeshGRegion::localQuality(
  std::vector<ovmVertexH> const &vertices, dtOVMMesh &ovm
) const
{
  // initialie quality struct
  LocalQuality result;
  result.minimum = std::numeric_limits<dtReal>::max();
  result.average = 0.0;
  result.minimumVolume = std::numeric_limits<dtReal>::max();
  result.minimumRelativeVolume = std::numeric_limits<dtReal>::max();
  result.nInvalid = 0;
  result.nReversed = 0;

  // calculate quality of each vertex; find global minimum quality, average,
  // minimum volumes, number of invalid elemenets and number of reversed
  // elements
  for (ovmVertexH const &vH : vertices)
  {
    LocalQuality const q = this->localQuality(vH, ovm);
    result.minimum = std::min(result.minimum, q.minimum);
    result.average += q.average;
    result.minimumVolume = std::min(result.minimumVolume, q.minimumVolume);
    result.minimumRelativeVolume =
      std::min(result.minimumRelativeVolume, q.minimumRelativeVolume);
    result.nInvalid += q.nInvalid;
    result.nReversed += q.nReversed;
  }

  // calculate global average
  if (!vertices.empty())
  {
    result.average /= static_cast<dtReal>(vertices.size());
  }
  // set to zero if vertices is an empty vector
  else
  {
    dt__warning(localQuality(), << "Empty vertices vector.");
    result.minimum = 0.0;
    result.average = 0.0;
    result.minimumVolume = 0.0;
    result.minimumRelativeVolume = 0.0;
  }

  return result;
}

bool dtHybridOptMeshGRegion::better(
  LocalQuality const &candidate, LocalQuality const &current
) const
{
  dtReal const eps = 1.e-8;

  // improvement for invalid elements
  if (candidate.nInvalid < current.nInvalid)
    return true;
  // deterioration for invalid elements
  if (candidate.nInvalid > current.nInvalid)
    return false;
  // improvement for reversed elements
  if (candidate.nReversed < current.nReversed)
    return true;
  // deterioration for reversed elements
  if (candidate.nReversed > current.nReversed)
    return false;
  // improvement for minimum quality
  if (candidate.minimum > current.minimum + eps)
    return true;
  // improvement in average quality, while minimum quality remains constant
  if (std::abs(candidate.minimum - current.minimum) < eps &&
      candidate.average > current.average + eps)
    return true;
  // improvement in minimumRelativeVolume, while minimum and average quality
  // remains constant
  if (std::abs(candidate.minimum - current.minimum) < eps &&
      std::abs(candidate.average - current.average) < eps &&
      candidate.minimumRelativeVolume > current.minimumRelativeVolume + eps)
    return true;
  // improvement in minimumVolume, while minimum quality, average quality, and
  // minimumRelativeVolume remains constant
  if (std::abs(candidate.minimum - current.minimum) < eps &&
      std::abs(candidate.average - current.average) < eps &&
      std::abs(
        candidate.minimumRelativeVolume - current.minimumRelativeVolume
      ) < eps &&
      candidate.minimumVolume > current.minimumVolume + eps)
    return true;

  // dt__warning(better(), << "Unhandled case.");
  return false;
}

dtPoint3 dtHybridOptMeshGRegion::extractVertexPosition(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  ::MVertex *mv = ovm[vH];
  return dtPoint3(mv->x(), mv->y(), mv->z());
}

dtPoint3 dtHybridOptMeshGRegion::calculateTetGoalPosition(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  dtPoint3 goal(0.0, 0.0, 0.0);
  dtReal totalWeight = 0.0;

  // calculate a volume weight of neighbouring tetrahedra
  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];
    if (!dynamic_cast<::MTetrahedron *>(me))
      continue;

    ::SPoint3 const bary = me->barycenter();
    dtVector3 const p(bary.x(), bary.y(), bary.z());
    dtReal const weight = std::abs(static_cast<dtReal>(me->getVolume()));

    goal = goal + weight * p;
    totalWeight = totalWeight + weight;
  }
  if (totalWeight > 0.0)
  {
    goal = dtPoint3(
      goal.x() / totalWeight, goal.y() / totalWeight, goal.z() / totalWeight
    );
  }
  else
  {
    dt__warning(calculateGoalPosition(), << "totalWeight<=0");
    goal = this->extractVertexPosition(vH, ovm);
  }
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

  // extract barycenters of tetrahedra, barycenter of pyramid, normal
  // direction of pyramid, and characteristic length of pyramid
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

  if (nTets == 0 || nPyrs != 1)
  {
    dt__warning(
      calculatePyramidGoalPosition(),
      << "Vertex with no neighbouring tetrahedra and more than one "
         "neighbouring pyramid"
    );
    return this->calculateTetGoalPosition(vH, ovm);
  }

  // correct direction of normal to ensure that pyramid is increased in the
  // direction of the tetrahedra
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

  return baryPyr + 0.5 * lPyr * dir * nPyr;
}

dtPoint3 dtHybridOptMeshGRegion::calculateGoalPosition(
  ovmVertexH const &vH, dtOVMMesh &ovm
) const
{
  // return goal position for vertex; a pyramid in the neighbourhood is
  // preferred, otherwise define goal positon by tetrahedra
  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    if (dynamic_cast<::MPyramid *>(ovm[*c_it]))
      return this->calculatePyramidGoalPosition(vH, ovm);
  }
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

  // get old quality and position
  dtPoint3 const oldPosition = this->extractVertexPosition(vH, ovm);
  LocalQuality const oldQuality = this->localQuality(vH, ovm);
  LocalQuality bestQuality = oldQuality;
  dtPoint3 bestPosition = oldPosition;

  // create candidates for improvement
  std::vector<dtPoint3> const candidates =
    this->extractCandidatePositions(vH, ovm);

  // check each candidate for quality and find best candidate
  bool relocate = false;
  for (std::size_t ii = 1; ii < candidates.size(); ++ii)
  {
    ovm.replacePosition(vH, candidates[ii]);
    LocalQuality const candidateQuality = this->localQuality(vH, ovm);
    if (this->better(candidateQuality, bestQuality))
    {
      relocate = true;
      bestQuality = candidateQuality;
      bestPosition = candidates[ii];
    }
  }

  // set position; if vertex was relocated, bestPosition contains new location;
  // if vertex was not relocated, bestPosition still keeps the old position
  ovm.replacePosition(vH, bestPosition);

  return relocate;
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

  // relocation iteration
  for (int iter = 0; iter < nIter; ++iter)
  {

    // try to relocate each movable vertex and count relocations
    int nRelocated = 0;
    dt__forAllRefAuto(vertices, vH)
    {
      if (this->relocateVertex(vH, ovm))
        ++nRelocated;
    }
    Msg::Info(
      "dtHybridOptMeshGRegion::relocateVertices() : iteration %d : relocated "
      "%d / %d vertices",
      iter,
      nRelocated,
      static_cast<int>(vertices.size())
    );

    // stop relocation iteration, it no further improvement possible
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

  std::vector<ovmVertexH> vertices;
  for (ovmCellVertexI v_it = ovm.cv_iter(cH); v_it.valid(); ++v_it)
  {
    vertices.push_back(*v_it);
  }

  if (vertices.size() != 4)
    return false;

  // find longest edge
  // todo: this could also be done with OVM edge iterators
  dtReal maxLengthSquared = std::numeric_limits<dtReal>::min();
  ovmVertexH v0;
  ovmVertexH v1;
  for (int i = 0; i < 4; ++i)
  {
    for (int j = i + 1; j < 4; ++j)
    {
      dtPoint3 const p0 = this->extractVertexPosition(vertices[i], ovm);
      dtPoint3 const p1 = this->extractVertexPosition(vertices[j], ovm);
      dtReal const lengthSquared = (p1 - p0).squared_length();
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

  // make sure that edge does not lie on a GFace
  if ((ovm[v0]->onWhat()->dim() < 3) && (ovm[v1]->onWhat()->dim() < 3))
  {
    return false;
  }
  // the complete edge neighbourhood must consist of tetrahedra
  // todo: this could also be done with OVM edge iterators
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

  LocalQuality const oldQuality = this->localQuality(v0, ovm);

  // do not split invalid or reversed elements
  if (oldQuality.nInvalid > 0 || oldQuality.nReversed > 0)
    return false;

  //
  // try the edge split; split only if no invalid, no reversed elements, no
  // zero volume cells, and no zero relative volume cells are created
  //
  return ovm.trySplitEdge(v0, v1, [&](ovmVertexH const &vNew) -> bool {
    LocalQuality const newQuality = this->localQuality(vNew, ovm);
    // never create invalid or reversed elements
    if (newQuality.nInvalid > 0 || newQuality.nReversed > 0)
      return false;

    // never create zero volume cells
    if (newQuality.minimumVolume <= 0.0 ||
        newQuality.minimumRelativeVolume <= 0.0)
      return false;

    return this->better(newQuality, oldQuality);
  });
}

bool dtHybridOptMeshGRegion::removeTet(ovmCellH const &cH, dtOVMMesh &ovm) const
{
  ::MElement *me = ovm[cH];
  if (!dynamic_cast<::MTetrahedron *>(me))
    return false;

  // find all neighbouring tetrahedra.
  std::vector<ovmCellH> neighbours;
  for (ovmCellCellI c_it = ovm.cc_iter(cH); c_it.valid(); ++c_it)
  {
    ovmCellH const cH2 = *c_it;
    ::MElement *me2 = ovm[cH2];
    if (!dynamic_cast<::MTetrahedron *>(me2))
      continue;
    neighbours.push_back(cH2);
  }

  if (neighbours.empty())
    return false;

  // try all neighbouring tetrahedra
  dt__forAllRefAuto(neighbours, neighbour)
  {
    // build the local five-vertex neighbourhood.
    std::vector<ovmVertexH> affected;
    for (ovmCellVertexI v_it = ovm.cv_iter(cH); v_it.valid(); ++v_it)
    {
      affected.push_back(*v_it);
    }
    for (ovmCellVertexI v_it = ovm.cv_iter(neighbour); v_it.valid(); ++v_it)
    {
      bool found = false;
      dt__forAllRefAuto(affected, vH)
      {
        if (*v_it == vH)
        {
          found = true;
          break;
        }
      }
      if (!found)
        affected.push_back(*v_it);
    }

    // a 2 -> 3 flip must have exactly five vertices.
    if (affected.size() != 5)
      continue;

    // store quality before the operation.
    LocalQuality const oldQuality = this->localQuality(affected, ovm);

    // do not operate on an already invalid local configuration.
    if (oldQuality.nInvalid > 0 || oldQuality.nReversed > 0)
      continue;

    // try the 2 -> 3 operation.
    bool const removed = ovm.tryRemoveTet(
      cH,
      neighbour,
      [&](std::vector<ovmVertexH> const &affectedVertices) {
        LocalQuality const newQuality =
          this->localQuality(affectedVertices, ovm);

        // never accept invalid or reversed tetrahedra.
        if (newQuality.nInvalid > 0 || newQuality.nReversed > 0)
          return false;

        // never accept zero-volume tetrahedra.
        if (newQuality.minimumVolume <= 0.0 ||
            newQuality.minimumRelativeVolume <= 0.0)
          return false;

        // accept only if the local quality improves.
        return this->better(newQuality, oldQuality);
      }
    );
    if (removed)
      return true;
  }

  return false;
}

void dtHybridOptMeshGRegion::optimizeTetrahedra(dtOVMMesh &ovm, int nIter) const
{
  dtReal const splitThreshold =
    config().lookupDef<dtReal>("_tetSplitThreshold", 0.10);

  // split iteration
  for (int iter = 0; iter < nIter; ++iter)
  {
    // init split and remove counter
    int nSplits = 0;
    int nRemoves = 0;

    // store tetrahedra in vector
    std::vector<ovmCellH> cells;
    for (ovmCellI c_it = ovm.c_iter(); c_it.valid(); ++c_it)
    {
      ovmCellH const cH = *c_it;
      if (dynamic_cast<::MTetrahedron *>(ovm[cH]))
        cells.push_back(cH);
    }

    // check each cell if a split improves the quality
    for (ovmCellH const &cH : cells)
    {
      // check if handle is valid and if MElement is not null; during split
      // operation some elements will be removed, so the pointer could be NULL
      if (!cH.is_valid())
        continue;
      ::MElement *me = ovm[cH];
      if (!me)
        continue;

      // split only if quality is below threshold
      dtReal const quality = me->minSICNShapeMeasure();
      if (quality >= splitThreshold)
        continue;

      // check if split improves quality
      if (this->splitTetEdge(cH, ovm))
      {
        ++nSplits;
        Msg::Info(
          "dtHybridOptMeshGRegion::optimizeTetrahedra() : "
          "split tetrahedron with quality %g",
          quality
        );
      }
      else if (this->removeTet(cH, ovm))
      {
        ++nRemoves;
        Msg::Info(
          "dtHybridOptMeshGRegion::optimizeTetrahedra() : "
          "remove tetrahedron with quality %g",
          quality
        );
      }
      // else
      //{
      //   Msg::Info(
      //     "dtHybridOptMeshGRegion::optimizeTetrahedra() : "
      //     "neither split nor remove of tetrahedron with quality %g "
      //     "was possible",
      //     quality
      //   );
      // }
      //  if (nSplits == 1) break;
    }

    // output
    Msg::Info(
      "dtHybridOptMeshGRegion::optimizeTetrahedra() : "
      "iteration %d : splits=%d removes=%d",
      iter,
      nSplits,
      nRemoves
    );

    // stop iteration if no split and no remove operation was performed
    if (nSplits == 0 && nRemoves == 0)
      break;
  }
}

} // namespace dtOO
