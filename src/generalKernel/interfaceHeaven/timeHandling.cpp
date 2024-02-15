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
