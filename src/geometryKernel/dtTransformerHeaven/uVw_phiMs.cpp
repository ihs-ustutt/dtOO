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

#include "uVw_phiMs.h"

#include "dtTransformerFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/analyticFunctionTransformed.h>
#include <analyticFunctionHeaven/vec2dMultiBiLinearTwoD.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
bool uVw_phiMs::_registrated =
  dtTransformerFactory::registrate(dt__tmpPtr(uVw_phiMs, new uVw_phiMs()));

uVw_phiMs::uVw_phiMs() : dtTransformer() {}

uVw_phiMs::uVw_phiMs(uVw_phiMs const &orig) : dtTransformer(orig)
{
  _rM2d.reset(orig._rM2d->clone());
  _ms_uSPercentVSPercent.reset(orig._ms_uSPercentVSPercent->clone());
}

uVw_phiMs::uVw_phiMs(jsonPrimitive const &jE) : dtTransformer(jE)
{
  this->jInit(jE, NULL, NULL, NULL, NULL);
}

uVw_phiMs::~uVw_phiMs() {}

dtTransformer *uVw_phiMs::clone(void) const { return new uVw_phiMs(*this); }

dtTransformer *uVw_phiMs::create(void) const { return new uVw_phiMs(); }

lvH_analyticFunction uVw_phiMs::apply(lvH_analyticFunction const *const aFP
) const
{
  lvH_analyticFunction retV;

  for (int ii = 0; ii < aFP->size(); ii++)
  {
    analyticFunction *aF = aFP->at(ii);

    retV.push_back(aF->cloneTransformed(this));
    retV.back()->setLabel(aF->getLabel());
  }

  return retV;
}

std::vector<dtPoint3>
uVw_phiMs::apply(std::vector<dtPoint3> const *const toTrans) const
{
  std::vector<dtPoint3> retVec;
  dtVector3 scale = config().lookupDef("_ss", dtVector3(1.0, 1.0, 1.0));
  dt__forAllIndex(*toTrans, ii)
  {
    dtReal phir = toTrans->at(ii).x() * scale.x();
    dtReal mm = std::max(toTrans->at(ii).y() * scale.y(), 0.);
    dtReal ss = toTrans->at(ii).z() * scale.z();
    dt__solution(ss > 1., ss = 1.);
    dt__solution(ss < 0., ss = 0.);

    dtReal wV = wV_ms(mm, ss);
    dtReal vV = vV_ms(mm, ss);
    dtReal uV = uV_phirVVWV(phir, vV, wV);

    retVec.push_back(dtPoint3(uV, vV, wV));
  }

  return retVec;
}

std::vector<dtPoint3>
uVw_phiMs::retract(std::vector<dtPoint3> const *const toRetract) const
{
  std::vector<dtPoint3> retVec;
  dtVector3 scale = config().lookupDef("_ss", dtVector3(1.0, 1.0, 1.0));
  dt__forAllIndex(*toRetract, ii)
  {
    dtReal uu = toRetract->at(ii).x() / scale.x();
    dtReal vv = toRetract->at(ii).y() / scale.y();
    dtReal ww = toRetract->at(ii).z() / scale.z();

    dtReal phir = phir_uVvVwV(uu, vv, ww);
    aFY ms = _ms_uSPercentVSPercent->Y(analyticFunction::aFXTwoD(
      _rM2d->constRefMap2dTo3d().percent_u(vv),
      _rM2d->constRefMap2dTo3d().percent_v(ww)
    ));
    dtReal mm = ms[0];
    dtReal ss = ms[1];

    dt__debug(
      retract(),
      << logMe::dtFormat("Retracting point (u, v, w) = (%E, %E, %E)") % uu %
             vv % ww
      << std::endl
      << logMe::dtFormat("to point (phir, mm, ss) = (%E, %E, %E)") % phir % mm %
             ss
    );

    retVec.push_back(dtPoint3(phir, mm, ss));
  }

  return retVec;
}

bool uVw_phiMs::isNecessary(void) const { return true; }

void uVw_phiMs::jInit(
  jsonPrimitive const &jE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::jInit(jE, bC, cV, aF, aG);

  dt__throwIf(!config().contains("_rM2d"), jInit());

  _rM2d = rotatingMap2dTo3d::PointerDownCast(
    config().lookupClone<analyticGeometry>("_rM2d", aG)
  );
  int nV = config().lookupDef<int>("_nV", 11);
  int nW = config().lookupDef<int>("_nW", 11);

  //
  // create piecewise bilinear mapping
  //
  twoDArrayHandling<dtPoint2> ms(nV, nW);
  dt__forAllIndex(ms, ii)
  {
    dt__forAllIndex(ms[ii], jj)
    {
      dtReal uPercent = static_cast<dtReal>(ii) / (nV - 1);
      dtReal vPercent = static_cast<dtReal>(jj) / (nW - 1);
      ms[ii][jj] = dtPoint2(
        m_uSVS(
          _rM2d->constRefMap2dTo3d().u_percent(uPercent),
          _rM2d->constRefMap2dTo3d().v_percent(vPercent)
        ),
        s_uSVS(
          _rM2d->constRefMap2dTo3d().u_percent(uPercent),
          _rM2d->constRefMap2dTo3d().v_percent(vPercent)
        )
      );
    }
  }
  _ms_uSPercentVSPercent.reset(new vec2dMultiBiLinearTwoD(ms));

  dt__info(
    init(),
    << dt__eval(_rM2d->constRefMap2dTo3d().getUMin()) << std::endl
    << dt__eval(_rM2d->constRefMap2dTo3d().getUMax()) << std::endl
    << dt__eval(_rM2d->constRefMap2dTo3d().getVMin()) << std::endl
    << dt__eval(_rM2d->constRefMap2dTo3d().getVMax()) << std::endl
    << dt__point2d(_ms_uSPercentVSPercent->YdtPoint2Percent(0., 0.))
    << std::endl
    << dt__point2d(_ms_uSPercentVSPercent->YdtPoint2Percent(1., 0.))
    << std::endl
    << dt__point2d(_ms_uSPercentVSPercent->YdtPoint2Percent(1., 1.))
    << std::endl
    << dt__point2d(_ms_uSPercentVSPercent->YdtPoint2Percent(0., 1.))
    << std::endl
  );
}

void uVw_phiMs::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::init(tE, bC, cV, aF, aG);

  jsonPrimitive config;

  if (dtXmlParserBase::hasChild("Vector_3", *tE))
  {
    ::QDomElement v3El = dtXmlParserBase::getChild("Vector_3", *tE);
    config.append<dtVector3>(
      "_ss", dtXmlParserBase::getDtVector3(&v3El, bC, cV, aF, aG)
    );
  }

  config.append<analyticGeometry const *>(
    "_rM2d", aG->get(dtXmlParserBase::getAttributeStr("part_label", *tE))
  );

  if (dtXmlParserBase::hasAttribute("number_points_two", *tE))
  {
    config.append<int>(
      "_nV",
      dtXmlParserBase::getAttributeIntMuParse("number_points_two", *tE, cV, aF)
    );
  }

  if (dtXmlParserBase::hasAttribute("number_points_three", *tE))
  {
    config.append<int>(
      "_nW",
      dtXmlParserBase::getAttributeIntMuParse(
        "number_points_three", *tE, cV, aF
      )
    );
  }
  jInit(config, bC, cV, aF, aG);
}

dtReal uVw_phiMs::m_uSVS(dtReal const &uu, dtReal const &vv) const
{
  ptrHandling<map1dTo3d> m1d(_rM2d->constRefMap2dTo3d().segmentConstV(vv));
  return m1d->l_u(uu);
}

dtReal uVw_phiMs::s_uSVS(dtReal const &uu, dtReal const &vv) const
{
  ptrHandling<map1dTo3d> m1d(_rM2d->constRefMap2dTo3d().segmentConstU(uu));

  return m1d->lPercent_u(vv);
}

dtReal uVw_phiMs::uV_phirVVWV(
  dtReal const &phir, dtReal const &vv, dtReal const &ww
) const
{
  return phir / (2. * M_PI);
}

dtReal uVw_phiMs::vV_ms(dtReal const &mm, dtReal const &ss) const
{
  aFY ms = analyticFunction::aFYTwoD(mm, ss);
  aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);

  return _rM2d->constRefMap2dTo3d().u_percent(uvPercent[0]);
}

dtReal uVw_phiMs::wV_ms(dtReal const &mm, dtReal const &ss) const
{
  aFY ms = analyticFunction::aFYTwoD(mm, ss);
  aFX uvPercent = _ms_uSPercentVSPercent->invY(ms);

  return _rM2d->constRefMap2dTo3d().v_percent(uvPercent[1]);
}

dtReal uVw_phiMs::phir_uVvVwV(
  dtReal const &uu, dtReal const &vv, dtReal const &ww
) const
{
  return uu * (2. * M_PI);
}
} // namespace dtOO
