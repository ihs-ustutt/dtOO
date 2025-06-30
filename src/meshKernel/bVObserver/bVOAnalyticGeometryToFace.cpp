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

#include "bVOAnalyticGeometryToFace.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshVertex.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include "bVOInterfaceFactory.h"
#include <analyticGeometryHeaven/aGBuilder/uv_map2dTo3dClosestPointToPoint.h>
#include <logMe/dtMacros.h>

namespace dtOO {
bool bVOAnalyticGeometryToFace::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVOAnalyticGeometryToFace, new bVOAnalyticGeometryToFace())
);

bVOAnalyticGeometryToFace::bVOAnalyticGeometryToFace() {}

bVOAnalyticGeometryToFace::~bVOAnalyticGeometryToFace() {}

void bVOAnalyticGeometryToFace::jInit(
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

  _m2d =
    map2dTo3d::PointerVectorDownCast(jE.lookupVecClone<analyticGeometry>("", aG)
    );
}

void bVOAnalyticGeometryToFace::init(
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

  //			<bVObserver
  //				name="bVOAnalyticGeometryToFace"
  //				analyticGeometry="
  //					{aS_dt_hub_0}
  //					{aS_ru_outlet_full}
  //				"
  //				increase_tolerance="1000."
  //			/>
  jsonPrimitive jE;
  jE.append<dtReal>(
    "_inc",
    dtXmlParserBase::getAttributeFloatMuParse(
      "increase_tolerance", element, cV, aF, 1.
    )
  );
  std::vector<analyticGeometry *> geos;
  dt__forAllRefAuto(
    qtXmlBase::getAttributeStrVector("analyticGeometry", element), anEl
  )
  {
    geos.push_back(aG->get(anEl));
  }
  jE.append<std::vector<analyticGeometry *>>("", geos);
  if (qtXmlBase::hasAttribute("facesPerEntry", element))
  {
    jE.append<std::vector<dtInt>>(
      "_facesPerEntry",
      qtXmlBase::getAttributeIntVector("facesPerEntry", element)
    );
  }
  bVOAnalyticGeometryToFace::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVOAnalyticGeometryToFace::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);
  ::std::vector<dtInt> facesPerEntry =
    config().lookup<::std::vector<dtInt>>("_facesPerEntry");
  logContainer<bVOAnalyticGeometryToFace> logC(logINFO, "preUpdate()");

  // dt__forAllRefAuto(_m2d, aM2d)
  dt__forAllIndex(_m2d, i)
  {
    map2dTo3d const &aM2d = _m2d[i];
    dtInt tagged = 0;
    logC() << aM2d.getLabel() << std::endl;

    //
    // local counter to prevent equal tags
    //
    dtInt localCounter = 0;

    //
    // initialize min distance to max float
    //
    dtReal minDistAv = std::numeric_limits<dtReal>::max();
    dt__forAllRefAuto(gm->dtFaces(), aFace)
    {
      dtInt inTol = 0;
      dtReal distAv = 0.;
      dt__forAllRefAuto(aFace->dtVertices(), aVertex)
      {
        dtReal dist = uv_map2dTo3dClosestPointToPoint(
                        &(aM2d), dtGmshModel::extractPosition(aVertex)
        )
                        .distance();

        //
        // summerize distances
        //
        distAv = distAv + dist;

        //
        // increase if in precision
        //
        if (analyticGeometry::inXYZTolerance(
              dist, config().lookupDef<dtReal>("_inc", 1.0)
            ))
        {
          inTol++;
        }
        // break if at least one point is not on the surface
        else
        {
          break;
        }
      }

      //
      // calculate average
      //
      distAv = distAv / aFace->vertices().size();

      //
      // store min distance
      //
      minDistAv = std::min(distAv, minDistAv);

      //
      // tag entity if in tolerance
      //
      if (inTol == aFace->vertices().size())
      {
        tagged = tagged + 1;
        std::string tagString(
          aM2d.getLabel() + "_" + stringPrimitive::intToString(localCounter)
        );
        logC() << "  > " << tagString << " ( " << distAv << " ) " << std::endl;
        gm->tagPhysical(aFace, tagString);
        localCounter++;
      }
    }
    logC() << "  > minDistAv = " << minDistAv << std::endl;
    if (!facesPerEntry.empty() && facesPerEntry[i] >= 0)
    {
      dt__throwIfWithMessage(
        facesPerEntry[i] != tagged,
        preUpdate(),
        << "tagged = " << tagged << std::endl
        << "facesPerEntry = " << facesPerEntry[i] << std::endl
        << "aM2d = " << aM2d.getLabel() << std::endl
      );
    }
  }
}
} // namespace dtOO
