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

#include "pickLengthPercentRange.h"

#include "dtTransformerFactory.h"
#include "geometryEngine/dtCurve.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>
#include <geometryEngine/dtSurface.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
bool pickLengthPercentRange::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(pickLengthPercentRange, new pickLengthPercentRange())
);

pickLengthPercentRange::pickLengthPercentRange() : dtTransformer() {}

pickLengthPercentRange::pickLengthPercentRange(
  const pickLengthPercentRange &orig
)
  : dtTransformer(orig)
{
}

pickLengthPercentRange::~pickLengthPercentRange() {}

dtTransformer *pickLengthPercentRange::clone(void) const
{
  dt__throw(clone(), << "Not yet implemented.");
}

dtTransformer *pickLengthPercentRange::create(void) const
{
  return new pickLengthPercentRange();
}

lvH_analyticGeometry
pickLengthPercentRange::apply(lvH_analyticGeometry const *const aGeoVecP) const
{
  lvH_analyticGeometry aGeoRet;

  for (int ii = 0; ii < aGeoVecP->size(); ii++)
  {
    analyticGeometry *aGP = aGeoVecP->at(ii)->clone();
    //
    // cast
    //
    analyticSurface *aSP;
    dt__mustCast(aGP, analyticSurface, aSP);

    //
    if (_createSplineCurve3d)
    {
      dt__throw(apply(), << "Not yet supported.");
    }
    else if (_createAnalyticSurface)
    {
      dt__pH(map1dTo3d) m1dU(aSP->segmentConstVPercent(0., 0., 1.));
      dt__ptrAss(analyticCurve * s3dU, analyticCurve::DownCast(m1dU.get()));
      dtReal u0 =
        s3dU->ptrConstDtCurve()->uPercent_lPercent(_lengthPercentOneStart);
      dtReal u1 =
        s3dU->ptrConstDtCurve()->uPercent_lPercent(_lengthPercentOneEnd);
      dt__pH(map1dTo3d) m1dV(aSP->segmentConstUPercent(0., 0., 1.));
      dt__ptrAss(analyticCurve * s3dV, analyticCurve::DownCast(m1dV.get()));
      dtReal v0 =
        s3dV->ptrConstDtCurve()->uPercent_lPercent(_lengthPercentTwoStart);
      dtReal v1 =
        s3dV->ptrConstDtCurve()->uPercent_lPercent(_lengthPercentTwoEnd);

      map2dTo3d *m2d =
        aSP->segmentRectanglePercent(dtPoint2(u0, v0), dtPoint2(u1, v1));
      delete aGP;
      aGP = m2d;
    }
    //      else {
    //        dt__THROW(apply(),
    //                << "Either _createSplineCurve3d or _createAnalyticSurface
    //                "
    //                << "should be true.");
    //      }
    aGeoRet.push_back(aGP);
  }

  return aGeoRet;
}

bool pickLengthPercentRange::isNecessary(void) const { return true; }

void pickLengthPercentRange::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::init(tE, bC, cV, aF, aG);

  _createSplineCurve3d = true;
  _createAnalyticSurface = false;

  if (dtXmlParserBase::hasAttribute("length_percent_one_start", *tE) &&
      dtXmlParserBase::hasAttribute("length_percent_two_start", *tE) &&
      dtXmlParserBase::hasAttribute("length_percent_one_end", *tE) &&
      dtXmlParserBase::hasAttribute("length_percent_two_end", *tE))
  {
    if (dtXmlParserBase::hasAttribute("length_percent_one_start", *tE))
    {
      _lengthPercentOneStart =
        dtXmlParserBase::muParseString(dtXmlParserBase::replaceDependencies(
          dtXmlParserBase::getAttributeStr("length_percent_one_start", *tE),
          cV,
          aF
        ));
    }

    if (dtXmlParserBase::hasAttribute("length_percent_two_start", *tE))
    {
      _lengthPercentTwoStart =
        dtXmlParserBase::muParseString(dtXmlParserBase::replaceDependencies(
          dtXmlParserBase::getAttributeStr("length_percent_two_start", *tE),
          cV,
          aF
        ));
    }

    if (dtXmlParserBase::hasAttribute("length_percent_one_end", *tE))
    {
      _lengthPercentOneEnd =
        dtXmlParserBase::muParseString(dtXmlParserBase::replaceDependencies(
          dtXmlParserBase::getAttributeStr("length_percent_one_end", *tE),
          cV,
          aF
        ));
    }

    if (dtXmlParserBase::hasAttribute("length_percent_two_end", *tE))
    {
      _lengthPercentTwoEnd =
        dtXmlParserBase::muParseString(dtXmlParserBase::replaceDependencies(
          dtXmlParserBase::getAttributeStr("length_percent_two_end", *tE),
          cV,
          aF
        ));
    }

    //
    // input check
    //
    if ((_lengthPercentOneStart == _lengthPercentOneEnd) &&
        (_lengthPercentTwoStart == _lengthPercentTwoEnd))
    {
      dt__throw(
        init(),
        << dt__eval(_lengthPercentOneStart) << std::endl
        << dt__eval(_lengthPercentTwoStart) << std::endl
        << dt__eval(_lengthPercentOneEnd) << std::endl
        << dt__eval(_lengthPercentTwoEnd) << std::endl
        << "Either lengthOneStart and lengthOneEnd or/and\n"
        << " lengthTwoStart and lengthTwoEnd should be different.\n"
        << " Both ranges equal means picking a point."
      );
    }
    if ((_lengthPercentOneStart != _lengthPercentOneEnd) &&
        (_lengthPercentTwoStart != _lengthPercentTwoEnd))
    {
      _createSplineCurve3d = false;
      _createAnalyticSurface = true;
    }
    if ((_lengthPercentOneStart > _lengthPercentOneEnd) ||
        (_lengthPercentTwoStart > _lengthPercentTwoEnd))
    {
      dt__throw(
        init(),
        << dt__eval(_lengthPercentOneStart) << std::endl
        << dt__eval(_lengthPercentTwoStart) << std::endl
        << dt__eval(_lengthPercentOneEnd) << std::endl
        << dt__eval(_lengthPercentTwoEnd) << std::endl
        << "Either lengthOneStart is bigger than lengthOneEnd or/and\n"
        << " lengthTwoStart is bigger than lengthTwoEnd.\n"
        << " In both ranges start should be smaller than end."
      );
    }
  }
  else
  {
    dt__throw(init(), << "Error in input values.");
  }
}
} // namespace dtOO
