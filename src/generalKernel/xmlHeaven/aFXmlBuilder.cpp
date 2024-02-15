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

#include "aFXmlBuilder.h"
#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>

namespace dtOO {
  aFXmlBuilder::aFXmlBuilder() {
  }

  void aFXmlBuilder::buildPartCompound(
    ::QDomElement const & toBuild, 
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticFunction * result
	) const {
    dt__info(buildPartCompound(), << "Call on aFXmlBuilder");
  }
	
  aFXmlBuilder::~aFXmlBuilder() {
  }
  
  std::vector< std::string > aFXmlBuilder::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }
}
