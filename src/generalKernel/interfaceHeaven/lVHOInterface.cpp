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

#include "lVHOInterface.h"

#include "lVHOSubject.h"
#include <interfaceHeaven/labelHandling.h>
#include <jsonHeaven/jsonPrimitive.h>
#include <logMe/logMe.h>

namespace dtOO {
lVHOInterface::lVHOInterface(jsonPrimitive const &config, lVHOSubject *subj)
{
  _subj = subj;
  // attach observer
  if (subj != NULL)
    subj->attach(this);
  // store config
  _config.reset(new jsonPrimitive(config));
  dt__debug(lVHOInterface(), << "config() = " << _config->toStdString());
}

lVHOInterface::~lVHOInterface() {}

void lVHOInterface::observeSet(labelHandling *lH)
{
  dt__debug(observeSet(), << "Nothing to do on: " << lH->getLabel());
}

void lVHOInterface::observe(labelHandling *lH)
{
  dt__debug(observe(), << "Nothing to do on: " << lH->getLabel());
}

jsonPrimitive &lVHOInterface::config(void) { return *_config; }

jsonPrimitive const &lVHOInterface::config(void) const { return *_config; }

lVHOSubject &lVHOInterface::subject(void)
{
  dt__throwIf(_subj == NULL, subject());
  return *_subj;
}

lVHOSubject const &lVHOInterface::subject(void) const
{
  dt__throwIf(_subj == NULL, subject());
  return *_subj;
}

bool lVHOInterface::typeObserves(lVHOSubject const &subj) const
{
  dt__throw(typeObserves(), << "Call");
}
} // namespace dtOO
