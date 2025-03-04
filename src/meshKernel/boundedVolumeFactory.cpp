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

#include "boundedVolumeFactory.h"

#include "boundedVolume.h"
#include <logMe/logMe.h>

namespace dtOO {
dt__pVH(boundedVolume) boundedVolumeFactory::_product;

boundedVolumeFactory::boundedVolumeFactory() {}

boundedVolumeFactory::~boundedVolumeFactory() {}

bool boundedVolumeFactory::registrate(boundedVolume const *const reg)
{
  dt__forAllRefAuto(_product, aProd)
  {
    if (aProd.virtualClassName() == reg->virtualClassName())
    {
      return false;
    }
  }
  _product.push_back(reg->create());

  return true;
}

boundedVolume *boundedVolumeFactory::create(char const *const str)
{
  return create(std::string(str));
}

boundedVolume *boundedVolumeFactory::create(std::string const str)
{
  dt__forAllRefAuto(_product, aProd)
  {
    //
    // check virtual class name
    //
    if (aProd.virtualClassName() == str)
    {
      return aProd.create();
    }

    //
    // check alias
    //
    dt__forAllRefAuto(aProd.factoryAlias(), anAlias)
    {
      if (anAlias == str)
        return aProd.create();
    }
  }

  std::vector<std::string> av;
  dt__forAllRefAuto(_product, aProd)
  {
    av.push_back(aProd.virtualClassName());
    dt__forAllRefAuto(aProd.factoryAlias(), anAlias)
    {
      av.push_back("  -> " + anAlias);
    }
  }
  dt__throw(
    create(),
    << str << " could not be created." << std::endl
    << "Implemented builder:" << std::endl
    << logMe::vecToString(av, 1) << std::endl
  );
}
} // namespace dtOO
