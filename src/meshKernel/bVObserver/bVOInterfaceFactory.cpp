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

#include "bVOInterfaceFactory.h"

#include "bVOInterface.h"
#include "bVOInterfaceFactory.h"
#include <logMe/logMe.h>
#include <string>

namespace dtOO {
dt__pVH(bVOInterface) bVOInterfaceFactory::_observer;

bVOInterfaceFactory::bVOInterfaceFactory() {}

bVOInterfaceFactory::~bVOInterfaceFactory() {}

bool bVOInterfaceFactory::registrate(bVOInterface const *const reg)
{
  dt__forAllRefAuto(_observer, anObs)
  {
    if (anObs.virtualClassName() == reg->virtualClassName())
    {
      return false;
    }
  }
  _observer.push_back(reg->create());

  return true;
}

bVOInterface *bVOInterfaceFactory::create(std::string const str)
{
  dt__forAllRefAuto(_observer, anObs)
  {
    //
    // check virtual class name
    //
    if (anObs.virtualClassName() == str)
    {
      return anObs.create();
    }

    //
    // check alias
    //
    dt__forAllRefAuto(anObs.factoryAlias(), anAlias)
    {
      if (anAlias == str)
        return anObs.create();
    }
  }

  std::vector<std::string> av;
  dt__forAllRefAuto(_observer, anObs)
  {
    av.push_back(anObs.virtualClassName());
    dt__forAllRefAuto(anObs.factoryAlias(), anAlias)
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

bVOInterface *bVOInterfaceFactory::create(char const *const str)
{
  return create(std::string(str));
}
} // namespace dtOO
