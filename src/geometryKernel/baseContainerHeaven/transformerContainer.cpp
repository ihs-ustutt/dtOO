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

#include "transformerContainer.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/logMe.h>
#include <vector>

namespace dtOO {
transformerContainer::transformerContainer() {}

transformerContainer::transformerContainer(const transformerContainer &orig) {}

transformerContainer::~transformerContainer() { _dtT.destroy(); }

void transformerContainer::add(dtTransformer const *const dtT)
{
  dt__info(add(), << "Add transformer " << dtT->getLabel());
  _dtT.push_back(dtT->clone());
}

dtTransformer const *transformerContainer::get(std::string const string) const
{
  return _dtT.get(string);
}

dtTransformer const *transformerContainer::operator[](std::string const label
) const
{
  return this->get(label);
}

bool transformerContainer::has(std::string const string) const
{
  return _dtT.has(string);
}

bool transformerContainer::isEmpty(void) const { return _dtT.empty(); }

std::vector<std::string> transformerContainer::labels(void) const
{
  return _dtT.labels();
}

labeledVectorHandling<dtTransformer *> &transformerContainer::lVH(void)
{
  return _dtT;
}

const labeledVectorHandling<dtTransformer *> &transformerContainer::lVH(void
) const
{
  return _dtT;
}
} // namespace dtOO
