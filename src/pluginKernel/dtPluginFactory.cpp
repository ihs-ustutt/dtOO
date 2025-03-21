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

#include "dtPluginFactory.h"

#include "dtPlugin.h"
#include "dtPluginDriver.h"
#include "dtPluginKernel.h"
#include <logMe/logMe.h>

namespace dtOO {
dt__pVH(dtPlugin) dtPluginFactory::_builder;

dtPluginFactory::~dtPluginFactory() {}

bool dtPluginFactory::registrate(dtPlugin const *const dtP)
{
  dt__forAllRefAuto(_builder, aBuilder)
  {
    if (aBuilder.virtualClassName() == dtP->virtualClassName())
    {
      return false;
    }
  }
  _builder.push_back(dtP->create());

  return true;
}

dtPlugin *dtPluginFactory::create(std::string const str)
{
  dt__debug(create(), << "str = " << str);
  dt__forAllRefAuto(_builder, aBuilder)
  {
    //
    // check virtual class name
    //
    if (aBuilder.virtualClassName() == str)
    {
      return aBuilder.create();
    }

    //
    // check alias
    //
    dt__forAllRefAuto(aBuilder.factoryAlias(), anAlias)
    {
      if (anAlias == str)
        return aBuilder.create();
    }
  }

  std::vector<std::string> av;
  dt__forAllRefAuto(_builder, aBuilder)
  {
    av.push_back(aBuilder.virtualClassName());
    dt__forAllRefAuto(aBuilder.factoryAlias(), anAlias)
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

dtPlugin *dtPluginFactory::createFromPlugin(
  std::string const &str,
  std::string const &pluginName,
  std::string const &pluginDriver
)
{
  dt__info(createFromPlugin(), << "creating " << str << "...");

  //
  // init kernel
  //
  dtPluginKernel *kernel = new dtPluginKernel();
  kernel->add_server(dtPlugin::server_name(), dtPlugin::version);

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
  dtPlugin *ret =
    kernel->get_driver<dtPluginDriver>(dtPlugin::server_name(), pluginDriver)
      ->create();

  //
  // store kernel in plugin
  //
  ret->setKernel(kernel);

  return ret;
}

dtPluginFactory::dtPluginFactory() {}
} // namespace dtOO
