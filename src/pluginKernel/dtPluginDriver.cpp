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

#include "dtPluginDriver.h"

#include "dtPlugin.h"

namespace dtOO {
dtPluginDriver::dtPluginDriver(std::string name, dtInt version)
  : pugg::Driver(dtPlugin::server_name(), name, version)
{
}

dtPluginDriver::~dtPluginDriver() {}
} // namespace dtOO
