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

#ifndef SOLID3DLINE_H
#define SOLID3DLINE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/renderInterface.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class solid3dLine : public renderInterface {
public:
  dt__class(solid3dLine, renderInterface);
  solid3dLine();
  solid3dLine(std::vector<dtPoint3> const &pp);
  virtual ~solid3dLine();
  std::vector<dtPoint3> const &refP3(void) const;

private:
  std::vector<dtPoint3> _p3;
};
} // namespace dtOO
#endif /* SOLID3DLINE_H */
