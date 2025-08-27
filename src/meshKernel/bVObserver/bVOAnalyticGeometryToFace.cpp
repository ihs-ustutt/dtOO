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
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
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
  jE.append<bool>(
    "_disableMidPoint",
    dtXmlParserBase::getAttributeBool("disable_midPoint", element, false)
  );
  jE.append<dtReal>(
    "_distanceCheckPoints",
    dtXmlParserBase::getAttributeFloatMuParse(
      "distance_checkpoints", element, cV, aF, 0.1
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

      //
      // define mid point and its distance
      //
      std::vector<dtPoint2> const &vertUVs = aFace->getVerticesUV();
      std::pair<dtPoint2, dtPoint2> uvBound =
        dtLinearAlgebra::boundingBox(vertUVs);
      dtPoint2 const midPointUV = dtPoint2(
        uvBound.first[0] + 0.5 * (uvBound.second[0] - uvBound.first[0]),
        uvBound.first[1] + 0.5 * (uvBound.second[1] - uvBound.first[1])
      );
      dtPoint3 const midPoint = aFace->getMap2dTo3d()->getPoint(midPointUV);
      dtReal const midDist =
        uv_map2dTo3dClosestPointToPoint(&(aM2d), midPoint).distance();
      bool const midIsOnFace = analyticGeometry::inXYZTolerance(
        midDist, config().lookupDef<dtReal>("_inc", 1.0)
      );

      dt__debug(
        preUpdate(),
        << logMe::dtFormat("Processing face: %s \n"
                           "  BouningBox (UV) : < (%f, %f), (%f, %f) >\n"
                           "  Vertices (UV) :\n"
                           "    1: (%f %f)  2: (%f %f)\n"
                           "    3: (%f %f)  4: (%f %f)\n"
                           "  MidPoint (UV) -> (XYZ) :(%f, %f) -> (%f, %f, %f)"
           ) % aM2d.getLabel() %
               uvBound.first[0] % uvBound.first[1] % uvBound.second[0] %
               uvBound.second[1] % midPointUV[0] % midPointUV[1] % midPoint[0] %
               midPoint[1] % midPoint[2] % vertUVs[0][0] % vertUVs[0][1] %
               vertUVs[1][0] % vertUVs[1][1] % vertUVs[2][0] % vertUVs[2][1] %
               vertUVs[3][0] % vertUVs[3][1]
      );

      //
      // define check points; prevent checking directly the corner points
      // (face vertices) as this is very unstable
      //
      dtReal const distanceCheckPoints =
        config().lookupDef<dtReal>("_distanceCheckPoints", 0.1);
      std::vector<dtPoint3> ckPoints;
      ckPoints.push_back(aFace->getMap2dTo3d()->getPoint(dtPoint2(
        uvBound.first[0] +
          distanceCheckPoints * (uvBound.second[0] - uvBound.first[0]),
        uvBound.first[1] +
          distanceCheckPoints * (uvBound.second[1] - uvBound.first[1])
      )));
      ckPoints.push_back(aFace->getMap2dTo3d()->getPoint(dtPoint2(
        uvBound.first[0] +
          (1.0 - distanceCheckPoints) * (uvBound.second[0] - uvBound.first[0]),
        uvBound.first[1] +
          distanceCheckPoints * (uvBound.second[1] - uvBound.first[1])
      )));
      ckPoints.push_back(aFace->getMap2dTo3d()->getPoint(dtPoint2(
        uvBound.first[0] +
          (1.0 - distanceCheckPoints) * (uvBound.second[0] - uvBound.first[0]),
        uvBound.first[1] +
          (1.0 - distanceCheckPoints) * (uvBound.second[1] - uvBound.first[1])
      )));
      ckPoints.push_back(aFace->getMap2dTo3d()->getPoint(dtPoint2(
        uvBound.first[0] +
          distanceCheckPoints * (uvBound.second[0] - uvBound.first[0]),
        uvBound.first[1] +
          (1.0 - distanceCheckPoints) * (uvBound.second[1] - uvBound.first[1])
      )));

      // check internal check points only if the mid point is on the surface
      // and the check is not disabled
      if (midIsOnFace || config().lookupDef("_disableMidPoint", false))
      {
        dt__forAllRefAuto(ckPoints, ckPoint)
        {
          dtReal dist =
            uv_map2dTo3dClosestPointToPoint(&(aM2d), ckPoint).distance();

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
      }

      // calculate average; if for loop was aborted the distance was calculated
      // for (inTol+1) points; therefore divide by (inTol+1)
      distAv = distAv / ::std::min<int>(inTol + 1, ckPoints.size());

      //
      // store min distance
      //
      minDistAv = std::min(distAv, minDistAv);

      //
      // tag entity if in tolerance
      //
      if (inTol == ckPoints.size())
      {
        dt__debug(
          preUpdate(),
          << logMe::dtFormat("Accept: midDist = %f, midIsOnFace = %d, %d == "
                             "%d, distAv = %f, label = %s") %
                 midDist % midIsOnFace % inTol % ckPoints.size() % distAv %
                 aM2d.getLabel()
        );

        tagged = tagged + 1;
        std::string tagString(
          aM2d.getLabel() + "_" + stringPrimitive::intToString(localCounter)
        );
        logC() << "  > " << tagString << " ( " << distAv << " ) " << std::endl;
        gm->tagPhysical(aFace, tagString);
        localCounter++;
      }
      else
      {
        dt__debug(
          preUpdate(),
          << logMe::dtFormat("Skip: midDist = %f, midIsOnFace = %d, %d == %d, "
                             "distAv = %f, label = %s") %
                 midDist % midIsOnFace % inTol % ckPoints.size() % distAv %
                 aM2d.getLabel()
        );
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
