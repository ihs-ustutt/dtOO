#include "dtTransformerDriver.h"

#include "dtTransformer.h"

namespace dtOO {
  dtTransformerDriver::dtTransformerDriver(
    std::string name, dtInt version
  ) : pugg::Driver(dtTransformer::server_name(), name, version) {
    
  }

  dtTransformerDriver::~dtTransformerDriver() {
  }
}
