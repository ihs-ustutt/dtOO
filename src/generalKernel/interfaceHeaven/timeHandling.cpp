#include "timeHandling.h"

#include <logMe/logMe.h>
#include <boost/timer/timer.hpp>

namespace dtOO {
  timeHandling::timeHandling( std::string const & keyword ) {
    _keyword = keyword;
    _t.reset( new ::boost::timer::cpu_timer() );
    _step = 0;
  }

  timeHandling::~timeHandling() {
  }
  
  void timeHandling::output( void ) {
    dt__info(
      measure(), 
      << _keyword << " ( " << _step << " ) " << _t->format()
    );
    _step = _step + 1;
  }
}

