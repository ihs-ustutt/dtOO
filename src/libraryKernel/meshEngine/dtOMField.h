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

#ifndef DTOMFIELD_H
#define DTOMFIELD_H

#include <dtOOTypeDef.h>

#include "dtOMMesh.h"
#include <interfaceHeaven/labelHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtOMField : public labelHandling {
public:
  dt__classOnlyName(dtOMField);
  dtOMField(std::string const &label, dtOMMesh const &om);
  virtual ~dtOMField();
  dtOMMesh const &refMesh(void) const;
  virtual void update(void) = 0;

private:
  dtOMMesh const &_om;
};
} // namespace dtOO
#endif /* DTOMFIELD_H */
