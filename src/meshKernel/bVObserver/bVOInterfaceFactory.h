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

#ifndef BVOINTERFACEFACTORY_H
#define BVOINTERFACEFACTORY_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class bVOInterface;

class bVOInterfaceFactory {
public:
  dt__classOnlyName(bVOInterfaceFactory);
  virtual ~bVOInterfaceFactory();
  static bool registrate(bVOInterface const *const);
  static bVOInterface *create(char const *const str);
  static bVOInterface *create(std::string const str);

private:
  bVOInterfaceFactory();

private:
  static dt__pVH(bVOInterface) _observer;
};
} // namespace dtOO
#endif /* BVOINTERFACEFACTORY_H */
