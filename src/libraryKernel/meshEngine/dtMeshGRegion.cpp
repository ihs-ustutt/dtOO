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
#include "qShapeMetric.h"
#include <gmsh/MElementOctree.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MTriangle.h>
#include <gmsh/meshGRegion.h>
#include <interfaceHeaven/barChart.h>
#include <progHelper.h>
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
  jE.append<dtReal>(
    "_relax",
    dtXmlParserBase::getAttributeFloatMuParse("relax", element, cV, aF)
  );
  jE.append<dtReal>(
    "_minQShapeMetric",
    dtXmlParserBase::getAttributeFloatMuParse(
      "minQShapeMetric", element, cV, aF
    )
  );
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
  jE.append<dtReal>(
    "_maxHeight",
    dtXmlParserBase::getAttributeFloatMuParse(
      "maxHeight", element, cV, aF, std::numeric_limits<dtReal>::max()
    )
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
    dt__forFromToIndex(0, config().lookup<dtInt>("_nSmooths"), ii)
    {
      dtOptimizeMeshGRegion()(dtgr);
    }

    dtgr->_status = ::GEntity::MeshGenerationStatus::DONE;
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
      SPoint3 bb = (*it)->barycenter();

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
  std::map<::MVertex *, dtReal> minQ_mv;
  dt__forAllRefAuto(vertices, aVert)
  {
    aVert->setEntity(dtgr);
    dtgr->addMeshVertex(aVert);
    minQ_mv[aVert] = std::numeric_limits<dtReal>::min();
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

  //
  // create barChart
  //
  barChart QTet_0("QTet_0", -1., 1., 30);
  dt__forAllRefAuto(dtgr->tetrahedra, aTet) QTet_0(qShapeMetric()(aTet));
  dt__info(createPyramids(), << QTet_0);

  dtOVMMesh ovm;
  createOVM(dtgr, ovm);

  //
  // first open
  //
  dt__forAllRefAuto(vertices, aVert)
  {
    ovmVertexH const &vH = ovm.at(aVert);
    dt__throwIf(!vH.is_valid(), createPyramids());

    dtPoint3 cC = dtGmshModel::extractPosition(aVert);

    //
    // set vertex position
    //
    std::vector<dtPoint3> pp = ovm.adjacentVertices(vH);
    dtPoint3 c1 = dtLinearAlgebra::toDtPoint3(
      (1. / pp.size()) * dtLinearAlgebra::sum(dtLinearAlgebra::toDtVector3(pp))
    );
    ovm.replacePosition(vH, cC + 0.01 * (c1 - cC));
  }
  dt__forAllRefAuto(pyramids, aPyr)
  {
    if (aPyr->getVolume() < 0.)
      aPyr->reverse();
  }

  //
  // get minimal shape metric
  //
  dtReal gMinQ = std::numeric_limits<dtReal>::max();
  dtReal gPyrMinQ = std::numeric_limits<dtReal>::max();
  dtReal gMinL = std::numeric_limits<dtReal>::max();
  dtReal gMaxL = std::numeric_limits<dtReal>::min();
  dt__forAllRefAuto(dtgr->tetrahedra, aTet)
  {
    gMinQ = std::min(qShapeMetric()(aTet), gMinQ);
    gMinL = std::min<dtReal>(aTet->minEdge(), gMinL);
    gMaxL = std::max<dtReal>(aTet->maxEdge(), gMaxL);
  }
  dt__forAllRefAuto(dtgr->pyramids, aPyr)
  {
    gMinQ = std::min(qShapeMetric()(aPyr), gMinQ);
    gPyrMinQ = std::min(qShapeMetric()(aPyr), gPyrMinQ);
  }

  //
  // pyramid open method
  //
  dt__forFromToIndex(0, config().lookup<dtInt>("_nPyramidOpenSteps"), ii)
  {
    dtInt vertMove = 0;
    dtInt vertFix = 0;
    dt__forAllRefAuto(vertices, aVert)
    {
      ovmVertexH const &vH = ovm.at(aVert);
      dt__throwIf(!vH.is_valid(), createPyramids());

      dtPoint3 cC = dtGmshModel::extractPosition(aVert);

      //
      // set vertex position
      //
      std::vector<dtPoint3> pp = ovm.adjacentVertices(vH);
      dtPoint3 c1 = dtLinearAlgebra::toDtPoint3(
        (1. / pp.size()) *
        dtLinearAlgebra::sum(dtLinearAlgebra::toDtVector3(pp))
      );
      ovm.replacePosition(
        vH, cC + config().lookup<dtReal>("_relax") * (c1 - cC)
      );

      dtReal pyrShape = std::numeric_limits<dtReal>::min();
      ::MPyramid *pyr = NULL;
      ;
      std::vector<dtReal> qq;
      for (ovmVertexCellI vcIt = ovm.vc_iter(vH); vcIt.valid(); ++vcIt)
      {
        qq.push_back(qShapeMetric()(ovm.at(*vcIt)));

        // tetrahedra
        if (ovm.at(*vcIt)->getNumVertices() == 4)
        {
          //
          // detect volume sign change --> element gets inverted
          //
          dtReal vol = ovm[*vcIt]->getVolume();
          dtReal iVol = ovm.request_cell_property<dtReal>("iV")[*vcIt];
          if ((iVol * vol) <= 0.)
          {
            qq[qq.size() - 1] = -1. * fabs(qq[qq.size() - 1]);
          }
        }
        // pyramid
        else
        {
          pyrShape = qq.back();
          pyr = dynamic_cast<::MPyramid *>(ovm[*vcIt]);
        }
      }

      //
      // check if step is not ok
      //
      // min shape metric should not decrease
      if (progHelper::min(qq) < gMinQ)
      {
        ovm.replacePosition(vH, cC);
        vertFix++;
      }
      // inverted element
      else if (progHelper::min(qq) < 0.)
      {
        ovm.replacePosition(vH, cC);
        vertFix++;
      }
      // pyramid violates maxHeight
      else if (pyramidHeight(pyr) >
               config().lookupDef<dtReal>(
                 "_maxHeight", std::numeric_limits<dtReal>::max()
               ))
      {
        // revert last step
        ovm.replacePosition(vH, cC);
        vertFix++;
      }
      else
      {
        gMinQ = std::min(progHelper::min(qq), gMinQ);
        gPyrMinQ = std::min(pyrShape, gPyrMinQ);
        minQ_mv[aVert] = progHelper::min(qq);
        vertMove++;
      }
    }
    logC(
    ) << logMe::dtFormat("%3i / %3i : %8i / %8i => Q = %8.2e / Q_pyr = %8.2e") %
           ii % config().lookup<dtInt>("_nPyramidOpenSteps") % vertMove %
           vertFix % gMinQ % gPyrMinQ
      << std::endl;
  }
  barChart QTet_1("QTet_1", -1., 1., 30);
  dt__forAllRefAuto(dtgr->tetrahedra, aTet)
  {
    QTet_1(fabs(qShapeMetric()(aTet)));
  }
  dt__info(createPyramids(), << QTet_1);

  barChart QPyr_1("QPyr_1", -1., 1., 30);
  dt__forAllRefAuto(dtgr->pyramids, aPyr)
  {
    QPyr_1(fabs(qShapeMetric()(aPyr)));
  }
  dt__info(createPyramids(), << QPyr_1);

  dt__forFromToIndex(0, config().lookup<dtInt>("_nSmooths"), ii)
  {
    dtOptimizeMeshGRegion()(dtgr);
  }

  barChart QTet_2("QTet_2", -1., 1., 30);
  dt__forAllRefAuto(dtgr->tetrahedra, aTet)
  {
    QTet_2(fabs(qShapeMetric()(aTet)));
  }
  dt__info(createPyramids(), << QTet_2);

  barChart QPyr_2("QPyr_2", -1., 1., 30);
  dt__forAllRefAuto(dtgr->pyramids, aPyr)
  {
    QPyr_2(fabs(qShapeMetric()(aPyr)));
  }
  dt__info(createPyramids(), << QPyr_2);
}

void dtMeshGRegion::createOVM(dtGmshRegion *dtgr, dtOVMMesh &ovm)
{
  //
  // create overall element vector
  //
  std::vector<::MElement *> me(dtgr->getNumMeshElements());
  dt__forFromToIndex(0, dtgr->getNumMeshElements(), ii)
  {
    me[ii] = dtgr->getMeshElement(ii);
  }

  //
  // create octree
  //
  ::MElementOctree oct(me);

  //
  // create OpenVolumeMesh
  //
  ::OpenVolumeMesh::CellPropertyT<dtReal> iV =
    ovm.request_cell_property<dtReal>("iV");
  ovm.set_persistent(iV);

  dtInt zeroVol = 0;
  dt__forAllRefAuto(dtgr->pyramids, aPyr)
  {
    dt__forFromToIndex(0, 5, ii)
    {
      std::vector<::MElement *> meVec = oct.findAll(
        aPyr->getVertex(ii)->x(),
        aPyr->getVertex(ii)->y(),
        aPyr->getVertex(ii)->z(),
        -1
      );
      dt__forAllRefAuto(meVec, aMe)
      {
        if (aMe->getVolume() == 0.)
          zeroVol++;
        ovmCellH cH = ovm.addCell(aMe);
        ovm.request_cell_property<dtReal>("iV")[cH] = aMe->getVolume();
      }
    }
  }
}

dtReal dtMeshGRegion::pyramidHeight(::MPyramid *pyr)
{
  SPoint3 bary = pyr->getFace(4).barycenter();
  return sqrt(
    (bary.x() - pyr->getVertex(4)->x()) * (bary.x() - pyr->getVertex(4)->x()) +
    (bary.y() - pyr->getVertex(4)->y()) * (bary.y() - pyr->getVertex(4)->y()) +
    (bary.z() - pyr->getVertex(4)->z()) * (bary.z() - pyr->getVertex(4)->z())
  );
}
} // namespace dtOO
