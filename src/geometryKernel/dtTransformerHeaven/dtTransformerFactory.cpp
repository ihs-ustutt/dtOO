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

#include "dtTransformerFactory.h"
#include "dtTransformer.h"
#include "dtTransformerDriver.h"
#include "dtTransformerKernel.h"
#include <logMe/logMe.h>

namespace dtOO {
dt__pVH(dtTransformer) dtTransformerFactory::_transformer;

dtTransformerFactory::dtTransformerFactory() {}

dtTransformerFactory::~dtTransformerFactory() {}

bool dtTransformerFactory::registrate(dtTransformer const *const reg)
{
  dt__forAllRefAuto(_transformer, aTrans)
  {
    if (aTrans.virtualClassName() == reg->virtualClassName())
    {
      return false;
    }
  }
  _transformer.push_back(reg->create());

  return true;
}

dtTransformer *dtTransformerFactory::create(std::string const str)
{
  dt__forAllRefAuto(_transformer, aTransformer)
  {
    //
    // check virtual class name
    //
    if (aTransformer.virtualClassName() == str)
    {
      return aTransformer.create();
    }

    //
    // check alias
    //
    dt__forAllRefAuto(aTransformer.factoryAlias(), anAlias)
    {
      if (anAlias == str)
        return aTransformer.create();
    }
  }

  std::vector<std::string> av;
  dt__forAllRefAuto(_transformer, aTrans)
  {
    av.push_back(aTrans.virtualClassName());
    dt__forAllRefAuto(aTrans.factoryAlias(), anAlias)
    {
      av.push_back("  -> " + anAlias);
    }
  }
  dt__throw(
    create(),
    << str << " could not be created." << std::endl
    << "Implemented transformer:" << std::endl
    << logMe::vecToString(av, 1) << std::endl
  );
}

dtTransformer *dtTransformerFactory::create(char const *const str)
{
  return create(std::string(str));
}

dtTransformer *dtTransformerFactory::createFromPlugin(
  std::string const &str,
  std::string const &pluginName,
  std::string const &pluginDriver
)
{
  dt__info(createFromPlugin(), << "creating " << str << "...");

  //
  // init kernel
  //
  dtTransformerKernel *kernel = new dtTransformerKernel();
  kernel->add_server(dtTransformer::server_name(), dtTransformer::version);

  //
  // output
  //
  dt__info(
    createFromPlugin(),
    << dt__eval(pluginName) << std::endl
    << dt__eval(pluginDriver)
  );

  //
  // load shared library
  //
  dt__throwIf(!kernel->load_plugin(pluginName), << "Loading plugin failed.");

  //
  // create plugin using plugin driver
  //
  dtTransformer *ret = kernel
                         ->get_driver<dtTransformerDriver>(
                           dtTransformer::server_name(), pluginDriver
                         )
                         ->create();

  //
  // store kernel in plugin
  //
  ret->setKernel(kernel);

  return ret;
}
} // namespace dtOO
