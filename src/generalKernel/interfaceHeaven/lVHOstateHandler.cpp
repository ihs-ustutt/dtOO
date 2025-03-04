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

#include "lVHOstateHandler.h"

#include <constValueHeaven/constValue.h>
#include <fstream>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <jsonHeaven/jsonPrimitive.h>
#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {
lVHOSubject *lVHOstateHandler::_subj(NULL);

lVHOstateHandler::lVHOstateHandler(void) : lVHOInterface(jsonPrimitive())
{
  dt__throwIfWithMessage(
    !lVHOstateHandler::initialized(),
    lVHOstateHandler(),
    << "No lVHOstateHandler() initialized. Please initialize this observer "
       "to the desired labeledVectorHandling< constValue * > object."
  );
}

lVHOstateHandler::lVHOstateHandler(
  jsonPrimitive const &config, lVHOSubject *subj
)
  : lVHOInterface(config, subj)
{
  if (lVHOstateHandler::initialized())
  {
    dt__throw(lVHOstateHandler(), << "Already initialized.");
  }
  _subj = subj;
}

lVHOstateHandler::~lVHOstateHandler() {}

bool lVHOstateHandler::initialized(void)
{
  return (lVHOstateHandler::_subj != NULL);
}

std::string lVHOstateHandler::commonState(void)
{
  std::vector<std::string> cStates;

  dt__forAllRefAuto(
    constValue::VectorMustDownCast(this->subject().internalStdVector()), cV
  )
  {
    if (cV->loadable())
      cStates.push_back(cV->getState());
  }
  progHelper::removeBastardTwins(cStates);

  dt__debug(commonState(), << cStates);

  if (cStates.size() == 1)
  {
    return cStates[0];
  }
  else
  {
    return std::string();
  }
}

void lVHOstateHandler::makeState(std::string const &label)
{
  std::vector<constValue *> cVs =
    constValue::VectorMustDownCast(this->subject().internalStdVector());
  dt__throwIf(cVs.size() == 0, makeState());

  dt__forAllRefAuto(cVs, cV) if (cV->loadable()) cV->setState(label);
}

std::string lVHOstateHandler::writeState(std::string label)
{
  if (label == "")
  {
    label = NowDateAndTime();
    this->makeState(label);
  }

  std::vector<constValue *> cV =
    constValue::VectorMustDownCast(this->subject().internalStdVector());

  jsonPrimitive()
    .append<std::vector<jsonPrimitive>>(
      "state",
      std::vector<jsonPrimitive>(
        1,
        jsonPrimitive()
          .append<std::string>("label", label)
          .append<std::vector<constValue *>>("", cV)
      )
    )
    .write(label + ".json");

  return label;
}

lVHOSubject &lVHOstateHandler::subject(void)
{
  dt__throwIf(lVHOstateHandler::_subj == NULL, subject());
  return *lVHOstateHandler::_subj;
}

lVHOSubject const &lVHOstateHandler::subject(void) const
{
  dt__throwIf(lVHOstateHandler::_subj == NULL, subject());
  return *lVHOstateHandler::_subj;
}

bool lVHOstateHandler::typeObserves(lVHOSubject const &subj) const
{
  dt__forAllRefAuto(subj.refObserver(), obs)
  {
    if (lVHOstateHandler::Is(&obs))
      return true;
  }
  return false;
}

void lVHOstateHandler::clear(void) { lVHOstateHandler::_subj = NULL; }
} // namespace dtOO
