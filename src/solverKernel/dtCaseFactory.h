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

#ifndef DTCASEFACTORY_H
#define DTCASEFACTORY_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
class dtCase;

class dtCaseFactory {
public:
  dt__classOnlyName(dtCaseFactory);
  dtCaseFactory();
  virtual ~dtCaseFactory();
  static bool registrate(dtCase const *const);
  static dtCase *create(std::string const str);

private:
  static dt__pVH(dtCase) _builder;
};
} // namespace dtOO

#endif /* DTCASEFACTORY_H */
