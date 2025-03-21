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

#ifndef dtTransformerDriver_H
#define dtTransformerDriver_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <pluginEngine/pugg/Driver.h>

namespace dtOO {
class dtTransformer;

class dtTransformerDriver : public ::pugg::Driver {
public:
  dt__classOnlyName(dtTransformerDriver);
  dtTransformerDriver(std::string name, dtInt version);
  ~dtTransformerDriver();
  virtual dtTransformer *create(void) = 0;

private:
};
} // namespace dtOO

#endif /* dtTransformerDriver_H */
