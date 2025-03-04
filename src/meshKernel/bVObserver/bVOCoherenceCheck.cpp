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

#include "bVOCoherenceCheck.h"

#include "bVOInterfaceFactory.h"
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/transformerContainer.h>
#include <boundedVolume.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MLine.h>
#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MTriangle.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshVertex.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVOCoherenceCheck::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVOCoherenceCheck, new bVOCoherenceCheck())
);

bVOCoherenceCheck::bVOCoherenceCheck() {}

bVOCoherenceCheck::~bVOCoherenceCheck() {}

void bVOCoherenceCheck::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  boundedVolume *attachTo
)
{
  bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);

  //		<bVObserver
  //			name="bVOCoherenceCheck"
  //			relative_tolerance="0.5"
  //		/>

  dt__info(init(), << dtXmlParserBase::convertToString(element));

  _relTol = std::numeric_limits<dtReal>::max();
  if (dtXmlParserBase::hasAttribute("relative_tolerance", element))
  {
    _relTol = dtXmlParserBase::getAttributeFloatMuParse(
      "relative_tolerance", element, cV
    );
  }
  _absTol = std::numeric_limits<dtReal>::max();
  if (dtXmlParserBase::hasAttribute("absolute_tolerance", element))
  {
    _absTol = dtXmlParserBase::getAttributeFloatMuParse(
      "absolute_tolerance", element, cV
    );
  }
}

void bVOCoherenceCheck::postUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  SBoundingBox3d bbox = gm->bounds();
  dtReal lc = bbox.empty() ? 1. : norm(SVector3(bbox.max(), bbox.min()));

  dt__info(
    postUpdate(),
    << "relTol = " << _relTol << std::endl
    << "absTol = " << _absTol << std::endl
    << "lc = " << lc << std::endl
    << "absTol / lc = " << _absTol / lc << std::endl
    << "==> min(absTol / lc, relTol) = " << std::min(_absTol / lc, _relTol)
  );

  gm->checkMeshCoherence(std::min(_absTol / lc, _relTol));
}
} // namespace dtOO
