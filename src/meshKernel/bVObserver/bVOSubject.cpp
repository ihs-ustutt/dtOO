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

#include "bVOSubject.h"

#include "bVOInterface.h"
#include <boundedVolume.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>

namespace dtOO {
bVOSubject::bVOSubject() {}

bVOSubject::~bVOSubject() { _observers.destroy(); }

void bVOSubject::attachBVObserver(bVOInterface *observer)
{
  _observers.push_back(observer);
}

void bVOSubject::preNotify(void)
{
  dt__forAllRefAuto(_observers, anObserver)
  {
    if (!anObserver->constRefBoundedVolume().isMeshed())
    {
      anObserver->preUpdate();
    }
  }
}

void bVOSubject::postNotify(void)
{
  dt__forAllRefAuto(_observers, anObserver) anObserver->postUpdate();
}

void bVOSubject::dump(void) const
{
  logContainer<bVOSubject> logC(TLogLevel::logINFO, "dump()");
  int cc = 0;
  dt__forAllRefAuto(_observers, anOb)
  {
    logC() << "[ " << cc << " ] : " << anOb->virtualClassName() << std::endl;
    cc++;
  }
}
} // namespace dtOO
