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

#ifndef DISCRETE3DPOINTS_H
#define DISCRETE3DPOINTS_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/renderInterface.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class discrete3dPoints : public renderInterface {
public:
  dt__class(discrete3dPoints, renderInterface);
  discrete3dPoints();
  discrete3dPoints(std::vector<dtPoint3> const &pp);
  virtual ~discrete3dPoints();
  void addPoints(std::vector<dtPoint3> const &pp);
  std::vector<dtPoint3> const &constRefP3(void) const;
  std::vector<dtPoint3> &refP3(void);

private:
  std::vector<dtPoint3> _p3;
};
} // namespace dtOO
#endif /* DISCRETE3DPOINTS_H */
