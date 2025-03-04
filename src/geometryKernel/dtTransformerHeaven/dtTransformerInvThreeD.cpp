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

#include "dtTransformerInvThreeD.h"

#include <QtXml/QDomElement>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <logMe/logMe.h>

namespace dtOO {
dtTransformerInvThreeD::dtTransformerInvThreeD() : dtTransformer() {}

dtTransformerInvThreeD::~dtTransformerInvThreeD() {}

dtTransformerInvThreeD::dtTransformerInvThreeD(
  dtTransformerInvThreeD const &orig
)
  : dtTransformer(orig)
{
}

dtTransformerInvThreeD::dtTransformerInvThreeD(jsonPrimitive const &jE)
  : dtTransformer(jE)
{
  this->jInit(jE, NULL, NULL, NULL, NULL);
}

dtVector3 dtTransformerInvThreeD::apply(dtVector3 const &pp) const
{
  std::vector<dtVector3> vec;
  vec.push_back(dtVector3(pp));

  std::vector<dtVector3> retVec = this->apply(&vec);

  return retVec[0];
}

dtVector3 dtTransformerInvThreeD::retract(dtVector3 const &pp) const
{
  std::vector<dtVector3> vec;
  vec.push_back(dtVector3(pp));

  std::vector<dtVector3> retVec = this->apply(&vec);

  return retVec[0];
}

dtPoint3 dtTransformerInvThreeD::apply(dtPoint3 const &pp) const
{
  std::vector<dtPoint3> vec;
  vec.push_back(dtPoint3(pp));

  std::vector<dtPoint3> retVec = this->apply(&vec);

  return retVec[0];
}

dtPoint3 dtTransformerInvThreeD::retract(dtPoint3 const &pp) const
{
  std::vector<dtPoint3> vec;
  vec.push_back(dtPoint3(pp));

  std::vector<dtPoint3> retVec = this->apply(&vec);

  return retVec[0];
}

std::vector<dtVector3>
dtTransformerInvThreeD::apply(std::vector<dtVector3> const &pp) const
{
  return apply(&pp);
}

std::vector<dtVector3>
dtTransformerInvThreeD::retract(std::vector<dtVector3> const &pp) const
{
  return retract(&pp);
}

std::vector<dtPoint3>
dtTransformerInvThreeD::apply(std::vector<dtPoint3> const &pp) const
{
  return apply(&pp);
}

std::vector<dtPoint3>
dtTransformerInvThreeD::retract(std::vector<dtPoint3> const &pp) const
{
  return retract(&pp);
}
dt__C_addCloneForpVH(dtTransformerInvThreeD);
} // namespace dtOO
