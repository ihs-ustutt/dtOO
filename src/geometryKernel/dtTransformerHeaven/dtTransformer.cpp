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

#include "dtTransformer.h"

#include <QtXml/QDomElement>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <logMe/logMe.h>
#include <pluginEngine/pugg/Kernel.h>

namespace dtOO {
dtTransformer::dtTransformer() : labelHandling(), optionHandling() {}

dtTransformer::~dtTransformer() {}

dtTransformer::dtTransformer(dtTransformer const &orig)
  : labelHandling(orig), optionHandling(orig)
{
  _config = orig._config;
}

dtTransformer::dtTransformer(jsonPrimitive const &jE)
{
  this->jInit(jE, NULL, NULL, NULL, NULL);
}

void dtTransformer::jInit(
  jsonPrimitive const &jE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  _config = jE;
  labelHandling::jInit(jE);
  optionHandling::jInit(jE);
  dt__debug(jInit(), << "config() = " << _config.toStdString());
}

jsonPrimitive &dtTransformer::config(void) { return _config; }

jsonPrimitive const &dtTransformer::config(void) const { return _config; }

void dtTransformer::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  if (dtXmlParserBase::hasAttribute("label", *tE))
  {
    labelHandling::setLabel(dtXmlParserBase::getAttributeStr("label", *tE));
  }
  optionHandling::init(*tE, bC, cV, aF, aG);
}

void dtTransformer::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cValP,
  lvH_analyticFunction const *const sFunP
)
{
  init(tE, bC, cValP, sFunP, NULL);
}

std::vector<std::string> dtTransformer::factoryAlias(void) const
{
  return std::vector<std::string>(0);
}

std::vector<dtPoint2>
dtTransformer::apply(std::vector<dtPoint2> const *const pointVecP) const
{
  dt__warning(apply(), << "Call on abstract class!");
  std::vector<dtPoint2> ret;
  dt__forAllRefAuto(*pointVecP, aP) ret.push_back(aP);
  return ret;
}

std::vector<dtPoint3>
dtTransformer::apply(std::vector<dtPoint3> const *const pointVecP) const
{
  dt__warning(apply(), << "Call on abstract class!");
  std::vector<dtPoint3> ret;
  dt__forAllRefAuto(*pointVecP, aP) ret.push_back(aP);
  return ret;
}

lvH_analyticFunction
dtTransformer::apply(lvH_analyticFunction const *const sFunP) const
{
  dt__warning(apply(), << "Call on abstract class!");
  lvH_analyticFunction ret;
  dt__forAllRefAuto(*sFunP, aF) ret.push_back(aF->clone());
  return ret;
}

lvH_analyticGeometry
dtTransformer::apply(lvH_analyticGeometry const *const sFunP) const
{
  dt__warning(apply(), << "Call on abstract class!");
  lvH_analyticGeometry ret;
  dt__forAllRefAuto(*sFunP, aG) ret.push_back(aG->clone());
  return ret;
}

dtPoint3 dtTransformer::apply(dtPoint3 const &pp) const
{
  std::vector<dtPoint3> vec;
  vec.push_back(dtPoint3(pp));

  std::vector<dtPoint3> retVec = this->apply(&vec);

  return retVec[0];
}

dtPoint2 dtTransformer::apply(dtPoint2 const &pp) const
{
  std::vector<dtPoint2> vec;
  vec.push_back(dtPoint2(pp));

  std::vector<dtPoint2> retVec = this->apply(&vec);

  return retVec[0];
}

analyticFunction *dtTransformer::apply(analyticFunction const *const sF) const
{
  lvH_analyticFunction vHIn;
  vHIn.push_back(const_cast<analyticFunction *>(sF));
  lvH_analyticFunction vHOut = this->apply(&vHIn);
  dt__throwIfWithMessage(
    vHOut.size() > 1, apply(), << "Size of vHOut = " << vHOut.size()
  );
  return vHOut[0];
}

analyticGeometry *dtTransformer::apply(analyticGeometry const *const aG) const
{
  lvH_analyticGeometry vHIn;
  vHIn.push_back(const_cast<analyticGeometry *>(aG));
  lvH_analyticGeometry vHOut = this->apply(&vHIn);

  return vHOut[0];
}

std::vector<dtPoint3>
dtTransformer::retract(std::vector<dtPoint3> const *const toRetract) const
{
  dt__throw(retract(), << "Call on base class.");
}

dtPoint3 dtTransformer::retract(dtPoint3 const &pp) const
{
  std::vector<dtPoint3> vec;
  vec.push_back(dtPoint3(pp));

  std::vector<dtPoint3> retVec = this->retract(&vec);

  return retVec[0];
}

std::vector<dtPoint2>
dtTransformer::retract(std::vector<dtPoint2> const *const toRetract) const
{
  dt__throw(retract(), << "Call on base class.");
}

dtPoint2 dtTransformer::retract(dtPoint2 const &pp) const
{
  std::vector<dtPoint2> vec;
  vec.push_back(dtPoint2(pp));

  std::vector<dtPoint2> retVec = this->retract(&vec);

  return retVec[0];
}

dtPoint3 dtTransformer::operator()(dtPoint3 const &pp, dtInt const dir) const
{
  if (dir == 1)
    return apply(pp);
  else if (dir == -1)
    return retract(pp);
  else
    dt__throwUnexpected(operator());
}

void dtTransformer::handleFloat(std::string const name, dtReal const value)
{
  dt__throw(
    handleFloat(),
    << "Could not handle:" << std::endl
    << dt__eval(name) << std::endl
    << dt__eval(value)
  );
}

void dtTransformer::handleInt(std::string const name, dtInt const value)
{
  dt__throw(
    handleInt(),
    << "Could not handle:" << std::endl
    << dt__eval(name) << std::endl
    << dt__eval(value)
  );
}

void dtTransformer::handleAnalyticGeometry(
  std::string const name, analyticGeometry const *value
)
{
  dt__throw(
    handleAnalyticGeometry(),
    << "Could not handle:" << std::endl
    << dt__eval(name) << std::endl
    << dt__eval(value) << std::endl
    << dt__eval(value->getLabel())
  );
}

void dtTransformer::handleAnalyticFunction(
  std::string const name, analyticFunction const *value
)
{
  dt__throw(
    handleAnalyticFunction(),
    << "Could not handle:" << std::endl
    << dt__eval(name) << std::endl
    << dt__eval(value) << std::endl
    << dt__eval(value->getLabel())
  );
}

void dtTransformer::handleDtPoint2(std::string const name, dtPoint2 const value)
{
  dt__throw(
    handleDtPoint2(),
    << "Could not handle:" << std::endl
    << dt__eval(name) << std::endl
    << dt__point2d(value)
  );
}

void dtTransformer::handleBool(std::string const name, bool const value)
{
  dt__throw(
    handleBool(),
    << "Could not handle:" << std::endl
    << dt__eval(name) << std::endl
    << dt__eval(value)
  );
}

void dtTransformer::handleDtVector3(
  std::string const name, dtVector3 const value
)
{
  dt__throw(
    handleDtVector3(),
    << "Could not handle:" << std::endl
    << dt__eval(name) << std::endl
    << dt__vector3d(value)
  );
}

void dtTransformer::handleDtVector2(
  std::string const name, dtVector2 const value
)
{
  dt__throw(
    handleDtVector2(),
    << "Could not handle:" << std::endl
    << dt__eval(name) << std::endl
    << dt__vector2d(value)
  );
}

void dtTransformer::setKernel(::pugg::Kernel *kernel) { _kernel.reset(kernel); }
dt__C_addCloneForpVH(dtTransformer);
} // namespace dtOO
