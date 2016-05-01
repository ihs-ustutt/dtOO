#include "dtPluginDriver.h"

#include "dtPlugin.h"

namespace dtOO {
  dtPluginDriver::dtPluginDriver(
    std::string name, int version
  ) : pugg::Driver(dtPlugin::server_name(), name, version) {
    
  }

  dtPluginDriver::~dtPluginDriver() {
  }
}
