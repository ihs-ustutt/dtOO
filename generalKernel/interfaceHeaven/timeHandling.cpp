#include "timeHandling.h"

#include <logMe/logMe.h>
#include <boost/timer/timer.hpp>

namespace dtOO {
  timeHandling::timeHandling( std::string const & keyword ) {
    _keyword = keyword;
    _t = new ::boost::timer::auto_cpu_timer(_stream);
  }

  timeHandling::~timeHandling() {
    delete _t;
    
    dt__info(
      ~timeHandling(), 
      << "time measurement" << std::endl
      << dt__eval(_keyword) << std::endl
      << dt__eval(_stream.str())
    );
  }
}

