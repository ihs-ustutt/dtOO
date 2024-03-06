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

#include "aGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

namespace dtOO {
  aGXmlBuilder::aGXmlBuilder() {
  }

  aGXmlBuilder::~aGXmlBuilder() {
  }

	void aGXmlBuilder::buildPartCompound(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result  
	) const {
    dt__info(buildPartCompound(), << "Call on aGXmlBuilder");		
	}

  std::vector< std::string > aGXmlBuilder::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }  
}
