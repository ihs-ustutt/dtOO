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
#include <analyticGeometryHeaven/multipleBoundedSurface.h>
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
  _aG = jE.lookupVecClone<analyticGeometry>("", aG);
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

  dt__forAllIndex(_aG, i)
  {
    map2dTo3d const *m2d = NULL;
    multipleBoundedSurface const *mBS = NULL;
    bool isMultiBounded = false;
    if (map2dTo3d::Is(&_aG[i]))
    {
      logC() << "Handling map2dTo3d " << _aG[i].getLabel() << ".\n";
      m2d = map2dTo3d::ConstDownCast(&_aG[i]);
    }
    else if (multipleBoundedSurface::Is(&_aG[i]))
    {
      logC() << "Handling multipleBoundedSurface " << _aG[i].getLabel()
             << ".\n";
      isMultiBounded = true;
      mBS = multipleBoundedSurface::DownCast(&_aG[i]);
      m2d = map2dTo3d::MustConstDownCast(mBS->surfaceConstPtr());
    }
    else
    {
      logC() << "Type not handled. Ignoring " << _aG[i].getLabel() << ".\n";
    }

    dtInt tagged = 0;

    // local counter to prevent equal tags
    dtInt localCounter = 0;

    // initialize min distance to max float
    dtReal minDistAv = std::numeric_limits<dtReal>::max();

    // loop through all faces of gmsh model
    //   Note: face is a face of the gmsh model; m2d is the face of the
    //         observer that was defined within the json file.
    dt__forAllRefAuto(gm->dtFaces(), aFace)
    {
      dtInt inTol = 0;
      dtReal distAv = 0.;

      //
      // define face's mid point and its distance
      //
      std::vector<dtPoint2> const &vertUVs = aFace->getVerticesOrderedUV();
      std::pair<dtPoint2, dtPoint2> uvBound =
        dtLinearAlgebra::boundingBox(vertUVs);
      dtPoint2 const midPointUV = dtLinearAlgebra::centerPoint(vertUVs);
      dt__throwIf(
        !dtLinearAlgebra::isInsidePolygon(midPointUV, vertUVs), preUpdate()
      );
      dtPoint3 const midPoint = aFace->getMap2dTo3d()->getPoint(midPointUV);
      // calculate distance of face's midpoint to m2d
      dtReal const midDist =
        uv_map2dTo3dClosestPointToPoint(m2d, midPoint).distance();
      bool midIsOnFace = analyticGeometry::inXYZTolerance(
        midDist, config().lookupDef<dtReal>("_inc", 1.0)
      );
      // for multipleBoundedSurface objects the midpoint has to be on m2d
      // and, additionally, has to be inside of the polygon; the polygon is
      // formed by the edges
      if (isMultiBounded)
      {
        bool const insidePolygon = mBS->insideInternalPolygon(midPointUV);
        midIsOnFace = midIsOnFace && insidePolygon;
        logC() << "    + midPoint of " << _aG[i].getLabel()
               << " : insidePolygon = " << insidePolygon
               << " -> midIsOnFace = " << midIsOnFace << std::endl;
      }

      dt__debug(
        preUpdate(),
        << logMe::dtFormat("Processing face: %s \n"
                           "  BouningBox (UV) : < (%f, %f), (%f, %f) >\n"
                           "  Vertices (UV) :\n"
                           "    1: (%f %f)  2: (%f %f)\n"
                           "    3: (%f %f)  4: (%f %f)\n"
                           "  MidPoint (UV) -> (XYZ) :(%f, %f) -> (%f, %f, %f)"
           ) % m2d->getLabel() %
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
      std::vector<dtPoint3> ckPoints = calcCheckPoints(aFace);

      // check internal check points only if the mid point is on the surface
      // and the check is not disabled
      if (midIsOnFace || config().lookupDef("_disableMidPoint", false))
      {
        dt__forAllRefAuto(ckPoints, ckPoint)
        {
          uv_map2dTo3dClosestPointToPoint distAlgo(m2d, ckPoint);
          dtReal dist = distAlgo.distance();
          bool inTolerance = analyticGeometry::inXYZTolerance(
            dist, config().lookupDef<dtReal>("_inc", 1.0)
          );

          // give a second chance to points with midIsOnFace; use first guess
          // and increase number of iterations
          if (midIsOnFace && !inTolerance)
          {
            dt__forFromToIndex(0, 3, repCounter)
            {
              dt__info(
                preUpdate(),
                << "Reparameterization [" << repCounter
                << "] fails. Best solution found was ("
                << m2d->percent_u(distAlgo.result().x()) << ", "
                << m2d->percent_v(distAlgo.result().y()) << ") with distance "
                << dist << std::endl
              );
              distAlgo = uv_map2dTo3dClosestPointToPoint(
                m2d, ckPoint, m2d->percent_uv(distAlgo.result()), 500
              );
              dist = distAlgo.distance();
              inTolerance = analyticGeometry::inXYZTolerance(
                dist, config().lookupDef<dtReal>("_inc", 1.0)
              );
              if (inTolerance)
                break;
              dt__info(preUpdate(), << "The saga continues ...");
            }
          }

          // summarize distances
          distAv = distAv + dist;

          // increase if in precision
          if (inTolerance)
          {
            inTol++;
          }
          // break if at least one point is not on the surface, but NOT
          // midIsOnFace
          else if (!midIsOnFace)
            break;
        }
      }

      // calculate average; if for loop was aborted the distance was calculated
      // for (inTol+1) points; therefore divide by (inTol+1)
      distAv = distAv / ::std::min<int>(inTol + 1, ckPoints.size());

      //
      // store min distance
      //
      minDistAv = std::min(distAv, minDistAv);

      // tag entity if in tolerance
      // condition for match is relaxed; only mid point and two additional
      // points have to be on the face
      if (inTol > 2)
      {
        dt__debug(
          preUpdate(),
          << logMe::dtFormat("Accept: midDist = %f, midIsOnFace = %d, %d == "
                             "%d, distAv = %f, label = %s") %
                 midDist % midIsOnFace % inTol % ckPoints.size() % distAv %
                 m2d->getLabel()
        );

        tagged = tagged + 1;
        std::string tagString(
          m2d->getLabel() + "_" + stringPrimitive::intToString(localCounter)
        );
        if (isMultiBounded)
        {
          logC() << "    + Adjust tag of mBS > " << m2d->getLabel() << " -> "
                 << mBS->getLabel() << std::endl;
          tagString =
            mBS->getLabel() + "_" + stringPrimitive::intToString(localCounter);
        }
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
                 m2d->getLabel()
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
        << "aM2d = " << m2d->getLabel() << std::endl
      );
    }
  }
}

std::vector<dtPoint3>
bVOAnalyticGeometryToFace::calcCheckPoints(dtGmshFace const *const aFace) const
{
  std::vector<dtPoint2> const &vertUVs = aFace->getVerticesOrderedUV();
  dtPoint2 centerPoint = dtLinearAlgebra::centerPoint(vertUVs);

  dtReal const dd = config().lookupDef<dtReal>("_distanceCheckPoints", 0.1);

  std::vector<dtPoint3> ckPoints;
  if (dtLinearAlgebra::isConvexPolygon(vertUVs))
  {
    dt__forAllRefAuto(vertUVs, vertUV)
    {
      dtPoint2 const ckPoint = vertUV + dd * (centerPoint - vertUV);
      dt__throwIf(
        !dtLinearAlgebra::isInsidePolygon(ckPoint, vertUVs), calcCheckPoints()
      );
      ckPoints.push_back(aFace->getMap2dTo3d()->getPoint(ckPoint));
      // clang-format off
      dt__debug(
        calcCheckPoints(),
        << logMe::dtFormat(
          "Add check point of convex polygon\n"
          "( %5.2e, %5.2e ) -> (%5.2e, %5.2e, %5.2e)"
        ) 
        % ckPoint[0] % ckPoint[1] 
        % ckPoints.back()[0] % ckPoints.back()[1] % ckPoints.back()[2]
      );
      // clang-format on
    }
  }
  else
    dt__throwUnexpected(calcCheckPoints());

  return ckPoints;
}

} // namespace dtOO
