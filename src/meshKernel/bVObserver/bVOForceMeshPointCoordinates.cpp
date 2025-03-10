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

#include "bVOForceMeshPointCoordinates.h"

#include <baseContainerHeaven/baseContainer.h>
#include <boundedVolume.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include "bVOInterfaceFactory.h"
#include <analyticFunctionHeaven/vec3dThreeD.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVOForceMeshPointCoordinates::_registrated =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOForceMeshPointCoordinates, new bVOForceMeshPointCoordinates())
  );

bVOForceMeshPointCoordinates::bVOForceMeshPointCoordinates() {}

bVOForceMeshPointCoordinates::~bVOForceMeshPointCoordinates() {}

void bVOForceMeshPointCoordinates::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  boundedVolume *attachTo
)
{
  dt__throwIf(
    !dtXmlParserBase::hasAttribute("coordinateFunction", element) ||
      !dtXmlParserBase::hasAttribute("faceLabel", element),
    init()
  );

  bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);

  //    <bVObserver
  //      name="bVOForceMeshPointCoordinates"
  //      coordinateFunction="f_forceNy"
  //      faceLabel="INGVRUDTOUT_SYMNY"
  //    />

  _faceLabel = dtXmlParserBase::getAttributeStrVector("faceLabel", element);

  _v3d.reset(
    vec3dThreeD::MustConstDownCast(
      aF->get(dtXmlParserBase::getAttributeStr("coordinateFunction", element))
    )
      ->clone()
  );
}

void bVOForceMeshPointCoordinates::postUpdate(void)
{
  dt__throwIf(!ptrBoundedVolume()->isMeshed(), postUpdate());

  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  dt__forAllRefAuto(_faceLabel, aLabel)
  {
    std::list<dtGmshFace *> theFaces = gm->getDtGmshFaceListByPhysical(aLabel);

    dt__forAllRefAuto(theFaces, aFace)
    {

      dt__forFromToIndex(0, aFace->getNumMeshElements(), ii)
      {
        std::vector<::MVertex *> verts;
        aFace->getMeshElement(ii)->getVertices(verts);
        dt__forAllRefAuto(verts, aVert)
        {
          dtPoint3 xyz = _v3d->YdtPoint3(aVert->x(), aVert->y(), aVert->z());

          aVert->setXYZ(xyz.x(), xyz.y(), xyz.z());
        }
      }
    }
  }
}
} // namespace dtOO
