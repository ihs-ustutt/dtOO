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

#include "discrete3dVector.h"

#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/logMe.h>

namespace dtOO {
discrete3dVector::discrete3dVector() : renderInterface() {}

discrete3dVector::~discrete3dVector() {}

discrete3dVector::discrete3dVector(
  std::vector<dtVector3> const &vv, std::vector<dtPoint3> const &pp
)
  : renderInterface()
{
  _v3 = vv;
  _p3 = pp;

  updateBB();
}

discrete3dVector::discrete3dVector(dtVector3 const &vv, dtPoint3 const &pp)
  : renderInterface()
{
  _v3.push_back(vv);
  _p3.push_back(pp);

  updateBB();
}

std::vector<dtVector3> const &discrete3dVector::refV3(void) const
{
  return _v3;
}

std::vector<dtPoint3> const &discrete3dVector::refP3(void) const { return _p3; }

void discrete3dVector::updateBB(void)
{
  std::vector<dtPoint3> vAsP = dtLinearAlgebra::toDtPoint3(_v3);
  vAsP.push_back(dtPoint3(0, 0, 0));
  std::pair<dtPoint3, dtPoint3> minMax = dtLinearAlgebra::boundingBox(vAsP);
  geoBoundMin(0, minMax.first.x());
  geoBoundMin(1, minMax.first.y());
  geoBoundMin(2, minMax.first.z());
  geoBoundMax(0, minMax.second.x());
  geoBoundMax(1, minMax.second.y());
  geoBoundMax(2, minMax.second.z());
}
} // namespace dtOO
