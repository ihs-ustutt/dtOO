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

#include "bVOAnalyticFunctionToFace.h"

#include "bVOInterfaceFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaThreeD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshVertex.h>
#include <xmlHeaven/qtXmlBase.h>

namespace dtOO {
bool bVOAnalyticFunctionToFace::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVOAnalyticFunctionToFace, new bVOAnalyticFunctionToFace())
);

bVOAnalyticFunctionToFace::bVOAnalyticFunctionToFace() {}

bVOAnalyticFunctionToFace::~bVOAnalyticFunctionToFace() {}

void bVOAnalyticFunctionToFace::init(
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

  //  	<bVObserver
  //      name="bVOAnalyticFunctionToFace"
  //			analyticFunction="aS_sym_nx_analyticFunction"
  //    />

  dt__forAllRefAuto(
    qtXmlBase::getAttributeStrVector("analyticFunction", element), anEl
  )
  {
    _sca.push_back(scaThreeD::MustConstDownCast(aF->get(anEl))->clone());
  }
}

void bVOAnalyticFunctionToFace::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  logContainer<bVOAnalyticFunctionToFace> logC(logINFO, "preUpdate()");
  dt__forAllRefAuto(_sca, aSca)
  {
    logC() << aSca.getLabel() << std::endl;

    //
    // local counter to prevent equal tags
    //
    dtInt localCounter = 0;

    dt__forAllRefAuto(gm->dtFaces(), aFace)
    {
      dtInt inTol = 0;
      dt__forAllRefAuto(aFace->dtVertices(), aVertex)
      {
        //
        // ask function if vertex is part of the face to tag
        //
        if (aSca.YFloat(aVertex->x(), aVertex->y(), aVertex->z()) != 0.)
        {
          inTol++;
        }
      }

      //
      // tag entity if all vertices of face are part of the function
      //
      if (inTol == aFace->vertices().size())
      {
        std::string tagString(
          aSca.getLabel() + "_" + stringPrimitive::intToString(localCounter)
        );
        logC() << "  > " << tagString << std::endl;
        gm->tagPhysical(aFace, tagString);
        localCounter++;
      }
    }
  }
}
} // namespace dtOO
