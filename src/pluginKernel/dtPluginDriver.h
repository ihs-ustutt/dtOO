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

#ifndef DTPLUGINDRIVER_H
#define DTPLUGINDRIVER_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <pluginEngine/pugg/Driver.h>

namespace dtOO {
class dtPlugin;

class dtPluginDriver : public ::pugg::Driver {
public:
  dt__classOnlyName(dtPluginDriver);
  dtPluginDriver(std::string name, dtInt version);
  ~dtPluginDriver();
  virtual dtPlugin *create(void) = 0;

private:
};
} // namespace dtOO

#endif /* DTPLUGINDRIVER_H */
