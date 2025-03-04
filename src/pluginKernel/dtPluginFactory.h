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

#ifndef DTPLUGINFACTORY_H
#define DTPLUGINFACTORY_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <mainConceptFwd.h>
#include <string>

namespace dtOO {
class dtPlugin;

class dtPluginFactory {
public:
  dt__classOnlyName(dtPluginFactory);
  virtual ~dtPluginFactory();
  static bool registrate(dtPlugin const *const);
  static dtPlugin *create(std::string const str);
  static dtPlugin *createFromPlugin(
    std::string const &className,
    std::string const &pluginName,
    std::string const &pluginDriver
  );

private:
  dtPluginFactory();

private:
  static dt__pVH(dtPlugin) _builder;
};
} // namespace dtOO

#endif /* DTPLUGINFACTORY_H */
