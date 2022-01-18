#include "dtPluginDriver.h"

#include "dtPlugin.h"

namespace dtOO {
  dtPluginDriver::dtPluginDriver(
    std::string name, dtInt version
  ) : pugg::Driver(dtPlugin::server_name(), name, version) {
    
  }

  dtPluginDriver::~dtPluginDriver() {
  }
}
