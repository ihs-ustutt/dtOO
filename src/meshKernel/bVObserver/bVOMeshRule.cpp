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

#include "bVOMeshRule.h"

#include "bVOInterfaceFactory.h"
#include <boundedVolume.h>
#include <interfaceHeaven/lVHOstateHandler.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtMesh0DOperator.h>
#include <meshEngine/dtMesh1DOperator.h>
#include <meshEngine/dtMesh2DOperator.h>
#include <meshEngine/dtMesh3DOperator.h>
#include <meshEngine/dtMeshGVertex.h>
#include <meshEngine/dtMeshOperator.h>
#include <meshEngine/dtMeshOperatorFactory.h>
#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
bool bVOMeshRule::_registrated =
  bVOInterfaceFactory::registrate(dt__tmpPtr(bVOMeshRule, new bVOMeshRule()));

bVOMeshRule::bVOMeshRule() {}

bVOMeshRule::~bVOMeshRule() { _meshOperator.destroy(); }

void bVOMeshRule::jInit(
  jsonPrimitive const &jE,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  boundedVolume *attachTo
)
{
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);

  dt__forAllRefAuto(
    jE.lookup<std::vector<jsonPrimitive>>("dtMeshOperator"), anOpJson
  )
  {
    dtMeshOperator *anOp =
      dtMeshOperatorFactory::create(anOpJson.lookup<std::string>("name"));
    anOp->jInit(anOpJson, bC, cV, aF, aG, bV, &_meshOperator);

    _meshOperator.push_back(anOp);
  }
}

void bVOMeshRule::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  boundedVolume *attachTo
)
{
  //
  // init bVOInterface
  //
  bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);

  //<bVObserver
  //  name="bVOMeshRule"
  //  rule="
  //    :
  //    {dtMeshGEdge(*)}
  //    :
  //    :
  //    {dtMeshGFace(*)}
  //    :
  //    :
  //    {dtMeshGRegionWithBoundaryLayer(REG1)}
  //    {dtMeshGRegion(REGI_1)}
  //    :
  //  "
  //>
  //  <dtMeshOperator
  //    name="dtMeshGEdge"
  //    label="dtMeshGEdge"
  //  />
  //  <dtMeshOperator
  //    name="dtMeshGFace"
  //    label="dtMeshGFace"
  //  />
  //  <dtMeshOperator
  //    name="dtMeshGRegion"
  //    label="dtMeshGRegion"
  //    pyramidHeightScale="0.25"
  //  />
  //  <dtMeshOperator
  //    name="dtMeshGRegionWithBoundaryLayer"
  //    label="dtMeshGRegionWithBoundaryLayer"
  //    spacing="{.050}{.125}{.225}"
  //    thickness="0.01"
  //    nSmoothingSteps="0"
  //    nShrinkingSteps="0"
  //    maxDihedralAngle="_pi*60./180."
  //    faceLabel="{HUB}"
  //    faceOrientation="{-1}"
  //    fixedFaceLabel="{INLET}"
  //    fixedFaceOrientation="{1}"
  //    slidableFaceLabel="{INTERNAL_3}"
  //    slidableFaceOrientation="{-1}"
  //  />
  //</bVObserver>

  std::vector<::QDomElement> meshOp =
    qtXmlPrimitive::getChildVector("dtMeshOperator", element);

  dt__forAllConstIter(std::vector<::QDomElement>, meshOp, it)
  {
    dtMeshOperator *anOp =
      dtMeshOperatorFactory::create(qtXmlBase::getAttributeStr("name", *it));
    anOp->init(*it, bC, cV, aF, aG, bV, &_meshOperator);

    _meshOperator.push_back(anOp);
  }

  jsonPrimitive jE;
  std::string rule = qtXmlPrimitive::getAttributeStr("rule", element);
  jE.append<std::vector<std::string>>(
    "_rule1D",
    qtXmlBase::convertToStringVector(
      "{", "}", qtXmlPrimitive::getStringBetweenAndRemove(":", ":", &rule)
    )
  );
  jE.append<std::vector<std::string>>(
    "_rule2D",
    qtXmlBase::convertToStringVector(
      "{", "}", qtXmlPrimitive::getStringBetweenAndRemove(":", ":", &rule)
    )
  );
  jE.append<std::vector<std::string>>(
    "_rule3D",
    qtXmlBase::convertToStringVector(
      "{", "}", qtXmlPrimitive::getStringBetweenAndRemove(":", ":", &rule)
    )
  );
  jE.append<std::vector<std::string>>(
    "_only", qtXmlBase::getAttributeStrVector("only", element)
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);

  dt__info(
    preUpdate(),
    << "rule = " << rule << std::endl
    << "_rule1D = " << config().lookup<std::vector<std::string>>("_rule1D")
    << std::endl
    << "_rule2D = " << config().lookup<std::vector<std::string>>("_rule2D")
    << std::endl
    << "_rule3D = " << config().lookup<std::vector<std::string>>("_rule3D")
    << std::endl
    << "_only = " << config().lookup<std::vector<std::string>>("_only")
  );
}

void bVOMeshRule::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  if (ptrBoundedVolume()->isMeshed())
    return;

  ::GModel::setCurrent(gm);

  //
  // create filename for debug output
  //
  std::string cFileName;
  if (optionHandling::debugTrue())
  {
    cFileName = lVHOstateHandler().commonState();
    if (cFileName != "")
    {
      cFileName = cFileName + "_";
    }
    cFileName = cFileName + ptrBoundedVolume()->getLabel() + "_debug.msh";
  }

  std::list<dtGmshEdge *> ee;
  std::list<dtGmshFace *> ff;
  std::list<dtGmshRegion *> rr;

  if (config().lookup<std::vector<std::string>>("_only").empty())
  {
    ee = dtGmshModel::cast2DtGmshEdge(gm->edges());
    ff = dtGmshModel::cast2DtGmshFace(gm->faces());
    rr = dtGmshModel::cast2DtGmshRegion(gm->regions());
  }
  else
  {
    dt__forAllRefAuto(
      config().lookup<std::vector<std::string>>("_only"), anOnly
    )
    {
      ::GEntity *ge = gm->getGEntityByPhysical(anOnly);

      //
      // dtGmshFace
      //
      if (dtGmshFace::DownCast(ge))
      {
        dtGmshFace *gf = dtGmshFace::SecureCast(ge);
        dt__forAllRefAuto(dtGmshModel::cast2DtGmshEdge(gf->edges()), anEdge)
          ee.push_back(anEdge);
        ff.push_back(gf);
      }
      //
      // dtGmshRegion
      //
      else if (dtGmshRegion::DownCast(ge))
      {
        dtGmshRegion *gr = dtGmshRegion::SecureCast(ge);
        dt__forAllRefAuto(dtGmshModel::cast2DtGmshFace(gr->faces()), aFace)
        {
          ff.push_back(aFace);
          dt__forAllRefAuto(
            dtGmshModel::cast2DtGmshEdge(aFace->edges()), anEdge
          ) ee.push_back(anEdge);
        }
        rr.push_back(gr);
      }
      else
        dt__throwUnexpected(preUpdate());
    }

    //
    // make unique
    //
    ee.unique();
    ff.unique();
    rr.unique();
  }

  gm->prepareToMesh();

  //
  // 0D
  //
  //    gm->mesh(0);
  std::list<dtGmshVertex *> vv = dtGmshModel::cast2DtGmshVertex(gm->vertices());

  dt__forAllRefAuto(vv, aVertex)
  {
    if ((aVertex->_status != ::GEntity::MeshGenerationStatus::DONE) &&
        (aVertex->getMeshMaster() == aVertex))
      dtMeshGVertex()(aVertex);
  }

  //
  // corresponding vertices
  //
  dt__forAllRefAuto(vv, aVertex)
  {
    if (aVertex->getMeshMaster() != aVertex)
    {
      dtMesh0DOperator::correspondingVertex(
        dtGmshModel::cast2DtGmshVertex(aVertex->getMeshMaster()), aVertex
      );
    }
  }

  gm->prepareToMesh();

  //
  // 1D
  //
  dt__forAllRefAuto(
    config().lookup<std::vector<std::string>>("_rule1D"), currentOperatorStr
  )
  {
    std::string currentGEntityStr =
      qtXmlBase::getStringBetweenAndRemove("(", ")", &currentOperatorStr);

    dt__ptrAss(
      dtMesh1DOperator * current1D,
      dtMesh1DOperator::DownCast(_meshOperator.get(currentOperatorStr))
    );

    dt__forAllRefAuto(ee, aEdge)
    {
      if (((aEdge->meshStatistics.status !=
            ::GEntity::MeshGenerationStatus::DONE) &&
           gm->matchWildCardPhysical(currentGEntityStr, aEdge)) &&
          (aEdge->getMeshMaster() == aEdge))
        (*current1D)(aEdge);
      if (!cFileName.empty())
        gm->writeMSH(cFileName, 2.2, false, true);
    }
  }

  //
  // copy slave edges
  //
  dt__forAllRefAuto(ee, aEdge)
  {
    if (aEdge->getMeshMaster() != aEdge)
    {
      dtMesh1DOperator::copyMesh(
        dtGmshModel::cast2DtGmshEdge(aEdge->getMeshMaster()), aEdge
      );
    }
  }

  gm->prepareToMesh();

  //
  // 2D
  //
  dt__forAllRefAuto(
    config().lookup<std::vector<std::string>>("_rule2D"), currentOperatorStr
  )
  {
    std::string currentGEntityStr =
      qtXmlBase::getStringBetweenAndRemove("(", ")", &currentOperatorStr);

    dt__ptrAss(
      dtMesh2DOperator * current2D,
      dtMesh2DOperator::DownCast(_meshOperator.get(currentOperatorStr))
    );

    //
    // determine entities
    //
    dt__forAllRefAuto(ff, aFace)
    {
      if ((aFace->meshStatistics.status != ::GEntity::MeshGenerationStatus::DONE
          ) &&
          gm->matchWildCardPhysical(currentGEntityStr, aFace) &&
          (aFace->getMeshMaster() == aFace))
        (*current2D)(aFace);
      if (!cFileName.empty())
        gm->writeMSH(cFileName, 2.2, false, true);
    }
  }
  //
  // copy slave faces
  //
  dt__forAllRefAuto(ff, aFace)
  {
    if (aFace->getMeshMaster() != aFace)
    {
      dtMesh2DOperator::copyMesh(
        dtGmshModel::cast2DtGmshFace(aFace->getMeshMaster()), aFace
      );
    }
  }

  gm->prepareToMesh();

  //
  // 3D
  //
  dt__forAllRefAuto(
    config().lookup<std::vector<std::string>>("_rule3D"), currentOperatorStr
  )
  {
    std::string currentGEntityStr =
      qtXmlBase::getStringBetweenAndRemove("(", ")", &currentOperatorStr);

    dt__ptrAss(
      dtMesh3DOperator * current3D,
      dtMesh3DOperator::DownCast(_meshOperator.get(currentOperatorStr))
    );
    dt__forAllRefAuto(rr, aReg)
    {
      if ((aReg->_status != ::GEntity::MeshGenerationStatus::DONE) &&
          gm->matchWildCardPhysical(currentGEntityStr, aReg) &&
          (aReg->getMeshMaster() == aReg))
        (*current3D)(aReg);
      if (!cFileName.empty())
        gm->writeMSH(cFileName, 2.2, false, true);
    }
  }

  ptrBoundedVolume()->setMeshed();
}
} // namespace dtOO
