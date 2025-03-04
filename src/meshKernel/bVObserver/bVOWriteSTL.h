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

#ifndef bVOWriteSTL_H
#define bVOWriteSTL_H

#include <dtOOTypeDef.h>

#include "bVOInterface.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class constValue;
class analyticFunction;
class analyticGeometry;

class bVOWriteSTL : public bVOInterface {
public:
  dt__class(bVOWriteSTL, bVOInterface);
  dt__classSelfCreate(bVOWriteSTL);
  bVOWriteSTL();
  virtual ~bVOWriteSTL();
  virtual void init(
    ::QDomElement const &element,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_boundedVolume const *const bV,
    boundedVolume *attachTo
  );
  virtual void postUpdate(void);

private:
  std::string _filename;
  static bool _registrated;
};
} // namespace dtOO
#endif /* bVOWriteSTL_H */
