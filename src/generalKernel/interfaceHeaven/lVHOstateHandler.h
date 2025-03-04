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

#ifndef lVHOstateHandler_H
#define lVHOstateHandler_H

#include "lVHOInterface.h"
#include "lVHOSubject.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <mainConceptFwd.h>

namespace dtOO {
class labelHandling;

class lVHOstateHandler : public lVHOInterface {
public:
  dt__class(lVHOstateHandler, lVHOInterface);
  lVHOstateHandler(void);
  lVHOstateHandler(jsonPrimitive const &config, lVHOSubject *subj);
  virtual ~lVHOstateHandler();
  static bool initialized(void);
  std::string commonState(void);
  void makeState(std::string const &label);
  std::string writeState(std::string label = "");
  virtual bool typeObserves(lVHOSubject const &subj) const;
  static void clear(void);

protected:
  virtual lVHOSubject &subject(void);
  virtual lVHOSubject const &subject(void) const;

private:
  static lVHOSubject *_subj;
};
} // namespace dtOO

#endif /* lVHOstateHandler_H */
