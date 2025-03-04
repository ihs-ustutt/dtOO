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

#ifndef BVOSUBJECT_H
#define BVOSUBJECT_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class bVOInterface;
class boundedVolume;

class bVOSubject {
public:
  dt__class(bVOSubject, bVOSubject);
  bVOSubject();
  virtual ~bVOSubject();
  void attachBVObserver(bVOInterface *observer);
  void preNotify(void);
  void postNotify(void);
  void dump(void) const;

private:
  vectorHandling<bVOInterface *> _observers;
};
} // namespace dtOO
#endif /* BVOSUBJECT_H */
