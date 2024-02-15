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

#include "aFJsonBuilder.h"
#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>

#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/labeledVectorHandling.h>

namespace dtOO {
  aFJsonBuilder::aFJsonBuilder() {
  }

  aFJsonBuilder::~aFJsonBuilder() {
  }

  lvH_analyticFunction aFJsonBuilder::buildPart( 
    jsonPrimitive const & config 
  ) const {
    return this->buildPart( config, NULL, NULL, NULL); 
  }
}
