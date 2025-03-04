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

#ifndef lVHOjsonLoad_H
#define lVHOjsonLoad_H

#include "lVHOInterface.h"
#include "lVHOSubject.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <mainConceptFwd.h>

namespace dtOO {
class labelHandling;

class lVHOjsonLoad : public lVHOInterface {
public:
  dt__class(lVHOjsonLoad, lVHOInterface);
  lVHOjsonLoad(void);
  lVHOjsonLoad(jsonPrimitive const &config, lVHOSubject *subj);
  virtual ~lVHOjsonLoad();
  virtual void observeSet(labelHandling *lH);

private:
  dt__pH(lvH_constValue) _cV;
};
} // namespace dtOO

#endif /* lVHOjsonLoad_H */
