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

#include "threadSafeRoot.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <TThread.h>
#include <TError.h>

namespace dtOO {
  threadSafeRoot::threadSafeRoot() {
    if ( staticPropertiesHandler::ompInParallel() ) {
      _thread.reset( new ::TThread() );
      if ( 
        staticPropertiesHandler::getInstance()->getOptionInt("root_printLevel") 
        ==
        0
      ) {
        gErrorIgnoreLevel = 1001;
      }
    }
  }

  threadSafeRoot::~threadSafeRoot() {
  }
}
