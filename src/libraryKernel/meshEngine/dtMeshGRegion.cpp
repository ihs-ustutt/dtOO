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

#include "dtMeshGRegion.h"

#include "dtGmshFace.h"
#include "dtGmshModel.h"
#include "dtGmshRegion.h"
#include "dtMeshOperatorFactory.h"
#include "dtOVMMesh.h"
#include "dtOptimizeMeshGRegion.h"
#include <dtLinearAlgebra.h>
#include <gmsh/MElementOctree.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MTriangle.h>
#include <gmsh/SPoint3.h>
#include <gmsh/meshGRegion.h>
#include <limits>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <vector>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool dtMeshGRegion::_registrated = dtMeshOperatorFactory::registrate(
  dt__tmpPtr(dtMeshGRegion, new dtMeshGRegion())
);

dtMeshGRegion::dtMeshGRegion() : dtMesh3DOperator() {}

dtMeshGRegion::dtMeshGRegion(const dtMeshGRegion &orig) : dtMesh3DOperator(orig)
{
}

dtMeshGRegion::~dtMeshGRegion() {}

void dtMeshGRegion::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  dtMesh3DOperator::init(element, bC, cV, aF, aG, bV, mO);

  jsonPrimitive jE;
  jE.append<dtInt>(
    "_nPyramidOpenSteps",
    dtXmlParserBase::getAttributeIntMuParse(
      "nPyramidOpenSteps", element, cV, aF
    )
  );
  jE.append<dtInt>(
    "_nSmooths",
    dtXmlParserBase::getAttributeIntMuParse("nSmooths", element, cV, aF)
  );
  dtMeshGRegion::jInit(jE, bC, cV, aF, aG, bV, mO);
}

void dtMeshGRegion::operator()(dtGmshRegion *dtgr)
{
  //
  // transfinite meshing
  //
  if (dtgr->meshAttributes.method == MESH_TRANSFINITE)
  {
    std::vector<::GRegion *> delauny;
    ::meshGRegion mr(delauny);
    mr(dtgr);
    MeshDelaunayVolume(delauny);

    dtgr->_status = ::GEntity::MeshGenerationStatus::DONE;

    return;
  }

  //
  // get number of quad faces
  //
  dtInt quadFaces = 0;
  dtInt quads = 0;
  dtInt elems = 0;
  dt__forAllRefAuto(dtGmshModel::cast2DtGmshFace(dtgr->faces()), gf)
  {
    quads = quads + gf->quadrangles.size();
    elems = elems + gf->getNumMeshElements();
    if (gf->quadrangles.size())
    {
      quadFaces++;
    }
  }

  if ((quads > 0) && (quads != elems))
  {
    dt__info(
      operator(),
      << "Bounding faces contain " << quads << " quadrangles and " << elems
      << " elements in general." << std::endl
      << "=> " << (dtReal)quads / (dtReal)elems << " % quadrangles" << std::endl
      << quadFaces << " faces meshed with quads found." << std::endl
      << "Performing createPyramids()."
    );
    createPyramids(dtgr);
  }
  else
  {
    dt__info(
      operator(),
      << "Bounding faces contain no quadrangles." << std::endl
      << "Performing normal meshing."
    );

    std::vector<::GRegion *> delauny;
    ::meshGRegion mr(delauny);
    mr(dtgr);
    MeshDelaunayVolume(delauny);

    dtgr->_status = ::GEntity::MeshGenerationStatus::DONE;
  }

  dt__forFromToIndex(0, config().lookup<dtInt>("_nSmooths"), ii)
  {
    dtOptimizeMeshGRegion opt;
    if (optionHandling::debugTrue())
      opt.setOption("debug", "true");
    opt(dtgr);
  }
}

void dtMeshGRegion::createPyramids(dtGmshRegion *dtgr)
{
  logContainer<dtMeshGRegion> logC(logINFO, "createPyramids()");

  std::map<dtGmshFace *, dtGmshFace *> pseudo_org;
  std::vector<::MPyramid *> pyramids;
  std::vector<::MVertex *> vertices;

  dt__forAllRefAuto(dtGmshModel::cast2DtGmshFace(dtgr->faces()), gf)
  {
    //
    // modify only quadrangle surfaces
    //
    if (gf->quadrangles.size() == 0)
      continue;

    logC() << "Face " << gf->tag() << " contains quadrangles." << std::endl
           << "Remove face temporarily." << std::endl;

    //
    // create new pseudo face
    //
    std::vector<dtInt> ori = gf->edgeOrientations();
    dtGmshFace *pseudo = new dtGmshFace(
      gf->model(),
      dtgr->refDtGmshModel().getMaxFaceTag() + 1,
      progHelper::vector2List(gf->edges()),
      ori
    );
    gf->model()->add(pseudo);

    //
    // store in mapping
    //
    pseudo_org[gf] = pseudo;

    //
    // replace face with pseudo face
    //
    dtgr->replaceFace(gf, pseudo);

    //
    // copy triangles
    //
    std::vector<::MTriangle *> const &tri = gf->triangles;
    dt__forAllConstIter(std::vector<::MTriangle *>, tri, it)
    {
      //
      // create new pseudo mesh elements (triangles)
      //
      pseudo->addTriangle(new ::MTriangle(
        (*it)->getVertex(0), (*it)->getVertex(1), (*it)->getVertex(2)
      ));

      //
      // copy old face vertices
      //
      dt__forFromToIndex(0, 2, ii)
      {
        if ((*it)->getVertex(ii)->onWhat()->dim() < 2)
          continue;
        pseudo->addMeshVertex((*it)->getVertex(ii));
      }
    }

    //
    // create pyramids
    //
    std::vector<::MQuadrangle *> const &quads = gf->quadrangles;
    dt__forAllConstIter(std::vector<::MQuadrangle *>, quads, it)
    {
      //
      // calculate barycenter, inner radius and normal
      //
      ::SPoint3 bb = (*it)->barycenter();

      //
      // create new mesh vertex
      //
      vertices.push_back(new ::MVertex(bb.x(), bb.y(), bb.z(), pseudo));
      pseudo->addMeshVertex(vertices.back());

      //
      // create new pseudo mesh elements (triangles)
      //
      pseudo->addTriangle(new ::MTriangle(
        (*it)->getVertex(0), (*it)->getVertex(1), vertices.back()
      ));
      pseudo->addTriangle(new ::MTriangle(
        (*it)->getVertex(1), (*it)->getVertex(2), vertices.back()
      ));
      pseudo->addTriangle(new ::MTriangle(
        (*it)->getVertex(2), (*it)->getVertex(3), vertices.back()
      ));
      pseudo->addTriangle(new ::MTriangle(
        (*it)->getVertex(3), (*it)->getVertex(0), vertices.back()
      ));

      //
      // copy old face vertices
      //
      dt__forFromToIndex(0, 3, ii)
      {
        if ((*it)->getVertex(ii)->onWhat()->dim() < 2)
          continue;
        pseudo->addMeshVertex((*it)->getVertex(ii));
      }

      pyramids.push_back(new ::MPyramid(
        (*it)->getVertex(0),
        (*it)->getVertex(1),
        (*it)->getVertex(2),
        (*it)->getVertex(3),
        vertices.back()
      ));
    }
    pseudo->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
  }

  logC() << "Region: " << std::endl << dtgr->dumpToString() << std::endl;

  //
  // call meshing again
  //
  this->operator()(dtgr);

  //
  // add mesh vertices and pyramids to old volume
  //
  dt__forAllRefAuto(vertices, aVert)
  {
    aVert->setEntity(dtgr);
    dtgr->addMeshVertex(aVert);
  }
  dt__forAllRefAuto(pyramids, aPyr) dtgr->addPyramid(aPyr);

  //
  // delete created pseudo GFace
  //
  dt__forAllRefAuto(pseudo_org, aPair)
  {
    //
    // clear mesh
    //
    aPair.second->mesh_vertices.clear();
    aPair.second->deleteMesh();

    //
    // replace face
    //
    dtgr->replaceFace(aPair.second, aPair.first);

    //
    // remove in model
    //
    dtgr->model()->remove(aPair.second);
  }

  dtOVMMesh ovm;
  createOVM(dtgr, ovm);
  ::OpenVolumeMesh::VertexPropertyT<int> nShifts =
    ovm.request_vertex_property<int>("nShifts", 0);
  ::OpenVolumeMesh::VertexPropertyT<dtPoint3> goalPosition =
    ovm.request_vertex_property<dtPoint3>("goalPosition");
  ::OpenVolumeMesh::VertexPropertyT<dtPoint3> orgPosition =
    ovm.request_vertex_property<dtPoint3>("orgPosition");

  //
  // test opening of pyramids to barycenter of adjacent tetrahedra; it is
  // necessary to make sure that the pyramid has a positive volume
  //
  dt__forAllRefAuto(dtgr->pyramids, aPyr)
  {
    MVertex *aVert = aPyr->getVertex(4);
    ovmVertexH const &vH = ovm.at(aVert);
    dt__throwIf(!vH.is_valid(), createPyramids());

    orgPosition[vH] = dtGmshModel::extractPosition(aVert);
    goalPosition[vH] = extractPyramidGoalPosition(vH, ovm);

    //
    // set vertex position
    //
    ovm.replacePosition(vH, goalPosition[vH]);
    if (aPyr->getVolume() < 0.)
      aPyr->reverse();
    ovm.replacePosition(vH, orgPosition[vH]);
  }

  //
  // pyramid open method
  //
  dt__forFromToIndex(0, config().lookup<dtInt>("_nPyramidOpenSteps"), ii)
  {
    dtReal const cRelax =
      float(ii + 1) / float(config().lookup<dtInt>("_nPyramidOpenSteps"));
    dtInt vertMove = 0;
    dtInt vertFix = 0;
    dt__forAllRefAuto(vertices, aVert)
    {
      ovmVertexH const &vH = ovm.at(aVert);
      dt__throwIf(!vH.is_valid(), createPyramids());

      dtPoint3 const cOrg = orgPosition.at(vH);
      dtPoint3 const cGoal = goalPosition.at(vH);
      dtPoint3 const cCur = dtGmshModel::extractPosition(ovm.at(vH));

      //
      // set vertex position
      //
      ovm.replacePosition(vH, cOrg + cRelax * (cGoal - cOrg));

      // check if any neighbor tetrahedra gets inverted by the shift
      if (validShift(vH, ovm))
      {
        vertMove++;
        nShifts[vH] = nShifts[vH] + 1;
      }
      // shift inverts at least one tetrahedra
      else
      {
        // retract last step
        ovm.replacePosition(vH, cCur);
        vertFix++;
      }
    }
    logC() << logMe::dtFormat("%3i / %3i ( %f ) : %8i / %8i") % ii %
                config().lookup<dtInt>("_nPyramidOpenSteps") % cRelax %
                vertMove % vertFix
           << std::endl;
  }

  //
  // make sure that all vertices are shifted one time; if not throw an
  // exception
  //
  dt__forAllRefAuto(vertices, aVert)
    dt__throwIf(nShifts.at(ovm.at(aVert)) == 0, createPyramids());

  dt__forFromToIndex(0, config().lookup<dtInt>("_nSmooths"), ii)
  {
    dtOptimizeMeshGRegion opt;
    if (optionHandling::debugTrue())
      opt.setOption("debug", "true");
    opt(dtgr);
  }
}

void dtMeshGRegion::createOVM(dtGmshRegion *dtgr, dtOVMMesh &ovm)
{
  //
  // create OpenVolumeMesh
  //
  ::OpenVolumeMesh::CellPropertyT<dtReal> iV =
    ovm.request_cell_property<dtReal>(
      "iVol", std::numeric_limits<dtReal>::infinity()
    );
  ovm.set_persistent(iV);

  dt__forFromToIndex(0, dtgr->getNumMeshElements(), ii)
  {
    ::MElement *aMe = dtgr->getMeshElement(ii);
    ovmCellH cH = ovm.addCell(aMe);
    iV[cH] = aMe->getVolume();
  }
}

dtPoint3
dtMeshGRegion::extractPyramidGoalPosition(ovmVertexH const &vH, dtOVMMesh &ovm)
{
  std::vector<dtPoint3> pp;
  int nTets = 0;
  int nPyrs = 0;
  dtVector3 n_pyr;
  dtPoint3 bary_pyr;
  dtReal l_pyr;

  // iterate neighbor cells of vertex
  for (ovmVertexCellI c_it = ovm.vc_iter(vH); c_it.valid(); ++c_it)
  {
    ::MElement *me = ovm[*c_it];
    //
    // store barycenter of neighbor tetrahedron in vector; increase tetrahedra
    // counter
    //
    if (dynamic_cast<::MTetrahedron *>(me))
    {
      nTets = nTets + 1;
      ::SPoint3 const bary = me->barycenter();
      pp.push_back(dtPoint3(bary.x(), bary.y(), bary.z()));
    }
    //
    // store barycenter, characterisitc length and normal of neighbor pyramid;
    // the normal is calculated orthogonal to pyramid's quadrangle; the
    // direction of the normal is "corrected" afterwards to point into the
    // direction of the maximum distance between quadrangle center and one
    // tetrahedron's bary center
    //
    else if (dynamic_cast<::MPyramid *>(me))
    {
      nPyrs = nPyrs + 1;
      dtPoint3 const v0 = dtGmshModel::extractPosition(me->getVertex(0));
      dtPoint3 const v1 = dtGmshModel::extractPosition(me->getVertex(1));
      dtPoint3 const v2 = dtGmshModel::extractPosition(me->getVertex(2));
      dtPoint3 const v3 = dtGmshModel::extractPosition(me->getVertex(3));
      // calculate normal direction
      n_pyr = dtLinearAlgebra::normalize(
        dtLinearAlgebra::crossProduct(v1 - v0, v3 - v0)
      );
      ::SPoint3 const bary = me->barycenter();
      bary_pyr = dtPoint3(bary.x(), bary.y(), bary.z());
      // calucate characterisitc pyramid length as the minimum edge length of
      // the pyramid's rectangle
      l_pyr = std::min(
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

  dt__throwIf(nTets == 0, extractPyramidGoalPosition());
  dt__throwIf(nPyrs != 1, extractPyramidGoalPosition());

  //
  // calculate dot product of distance tetrahedron's barycenter to rectangle's
  // centroid
  //
  std::vector<dtReal> ss;
  dt__forAllRefAuto(pp, aP)
  {
    ss.push_back(dtLinearAlgebra::dotProduct(n_pyr, aP - bary_pyr));
  }

  //
  // find max and min value of dot products
  //
  dtReal maxS = *std::max_element(ss.begin(), ss.end());
  dtReal minS = *std::min_element(ss.begin(), ss.end());

  //
  // barycenter with the maximum absolute values defines the "correct side";
  // store direction in dir as +1.0 or -1.0
  //
  dtReal dir = 1.0;
  if (fabs(maxS) > fabs(minS))
  {
    if (maxS < 0.0)
      dir = -1.0;
  }
  else
  {
    if (minS < 0.0)
      dir = -1.0;
  }

  dtPoint3 const cGoal = bary_pyr + l_pyr / 2.0 * dir * n_pyr;

  Msg::Debug(
    "vH = %d / nTets = %d / pp.size() = %d / %f %f %f -> %f %f %f / dir = %f / "
    "l_pyr = %f",
    vH,
    nTets,
    pp.size(),
    ovm[vH]->x(),
    ovm[vH]->y(),
    ovm[vH]->z(),
    cGoal.x(),
    cGoal.y(),
    cGoal.z(),
    dir,
    l_pyr
  );

  return cGoal;
}

bool dtMeshGRegion::validShift(ovmVertexH const vH, dtOVMMesh &ovm)
{
  // check if any neighbor tetrahedra gets inverted by the shift
  for (ovmVertexCellI vcIt = ovm.vc_iter(vH); vcIt.valid(); ++vcIt)
  {
    // tetrahedra
    if (ovm.at(*vcIt)->getNumVertices() == 4)
    {
      //
      // detect volume sign change --> element gets inverted
      //
      dtReal vol = ovm[*vcIt]->getVolume();
      dtReal iVol = ovm.request_cell_property<dtReal>("iVol")[*vcIt];
      if ((iVol * vol) <= 0.)
      {
        return false;
      }
    }
  }
  return true;
}
} // namespace dtOO
