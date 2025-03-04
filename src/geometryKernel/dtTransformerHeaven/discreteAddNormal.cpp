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

#include "discreteAddNormal.h"

#include "dtTransformerFactory.h"
#include <analyticFunctionHeaven/aFBuilder/vec3dTwoD_normalOffset.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <baseContainerHeaven/baseContainer.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
bool discreteAddNormal::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(discreteAddNormal, new discreteAddNormal())
);

discreteAddNormal::discreteAddNormal() : dtTransformer() {}

discreteAddNormal::discreteAddNormal(discreteAddNormal const &orig)
  : dtTransformer(orig)
{
  _tt.reset(orig._tt->clone());
}

discreteAddNormal::discreteAddNormal(jsonPrimitive const &jE)
{
  this->jInit(jE, NULL, NULL, NULL, NULL);
}

discreteAddNormal::~discreteAddNormal() {}

dtTransformer *discreteAddNormal::clone(void) const
{
  return new discreteAddNormal(*this);
}

dtTransformer *discreteAddNormal::create(void) const
{
  return new discreteAddNormal();
}

lvH_analyticFunction
discreteAddNormal::apply(lvH_analyticFunction const *const aFP) const
{
  lvH_analyticFunction retV;

  dt__forAllIndex(*aFP, ii)
  {
    analyticFunction *aF = aFP->at(ii);

    vec3dTwoD const *const vec3d2d = vec3dTwoD::ConstDownCast(aF);
    if (vec3d2d)
    {
      vec3dTwoD_normalOffset anOffset(
        vec3d2d,
        _tt.get(),
        config().lookup<dtVector3>("_nf"),
        config().lookup<dtInt>("_nU"),
        config().lookup<dtInt>("_nV"),
        config().lookup<dtInt>("_order"),
        config().lookupDef<dtInt>("_skinOrderMin", 1),
        config().lookupDef<dtInt>("_skinOrderMax", 25),
        config().lookupDef<dtInt>("_skinNIterations", 0),
        config().lookupDef<bool>("_closeU", false),
        config().lookupDef<dtReal>("_closeSmooth", -1.0)
      );
      retV.push_back(anOffset.result());

      //
      // append wire if debug option set
      //
      if (optionHandling::debugTrue())
      {
        dt__info(apply(), << "Debug option set. Appending wire.")
          dt__forAllRefAuto(anOffset.resultWire(), aCurve)
        {
          retV.push_back(aCurve.clone());
        }
      }
    }
    else
      dt__throwUnexpected(apply());
  }

  return retV;
}

void discreteAddNormal::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::init(tE, bC, cV, aF, aG);

  dt__throwIf(
    !dtXmlParserBase::hasAttribute("function_label", *tE) ||
      !dtXmlParserBase::hasAttribute("number_points_one", *tE) ||
      !dtXmlParserBase::hasAttribute("number_points_two", *tE) ||
      !dtXmlParserBase::hasAttribute("order", *tE) ||
      !dtXmlParserBase::hasAttribute("nf", *tE),
    init()
  );
  jsonPrimitive config;
  //
  // get function
  //
  config.append<analyticFunction const *>(
    "_tt", aF->get(dtXmlParserBase::getAttributeStr("function_label", *tE))
  );

  //
  // get number of points
  //
  config.append<dtInt>(
    "_nU",
    dtXmlParserBase::getAttributeIntMuParse("number_points_one", *tE, cV, aF)
  );
  config.append<dtInt>(
    "_nV",
    dtXmlParserBase::getAttributeIntMuParse("number_points_two", *tE, cV, aF)
  );

  //
  // get order
  //
  config.append<dtInt>(
    "_order", dtXmlParserBase::getAttributeIntMuParse("order", *tE, cV, aF)
  );
  if (dtXmlParserBase::hasAttribute("skinOrderMin", *tE))
  {
    config.append<dtInt>(
      "_skinOrderMin",
      dtXmlParserBase::getAttributeIntMuParse("skinOrderMin", *tE, cV, aF)
    );
  }
  if (dtXmlParserBase::hasAttribute("skinOrderMax", *tE))
  {
    config.append<dtInt>(
      "_skinOrderMax",
      dtXmlParserBase::getAttributeIntMuParse("skinOrderMax", *tE, cV, aF)
    );
  }
  if (dtXmlParserBase::hasAttribute("skinNIterations", *tE))
  {
    config.append<dtInt>(
      "_skinNIterations",
      dtXmlParserBase::getAttributeIntMuParse("skinNIterations", *tE, cV, aF)
    );
  }

  //
  // get flag if close
  //
  if (dtXmlParserBase::hasAttribute("closeU", *tE))
  {
    config.append<bool>(
      "_closeU", dtXmlParserBase::getAttributeBool("closeU", *tE)
    );
  }
  //
  // get flag if close smooth
  //
  if (dtXmlParserBase::hasAttribute("closeSmooth", *tE))
  {
    config.append<dtReal>(
      "_closeSmooth",
      dtXmlParserBase::getAttributeFloatMuParse("closeSmooth", *tE, cV, aF)
    );
  }
  //
  // get vector
  //
  config.append<dtVector3>(
    "_nf",
    dtXmlParserBase::getDtVector3(
      dtXmlParserBase::getAttributeStr("nf", *tE), bC
    )
  );
  jInit(config, bC, cV, aF, aG);
}

void discreteAddNormal::jInit(
  jsonPrimitive const &jE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::jInit(jE, bC, cV, aF, aG);
  _tt =
    vec3dTwoD::PointerDownCast(config().lookupClone<analyticFunction>("_tt", aF)
    );
}

bool discreteAddNormal::isNecessary(void) const { return true; }
} // namespace dtOO
