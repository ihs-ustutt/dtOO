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

#include "dtPlugin.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <pluginEngine/pugg/Kernel.h>

namespace dtOO {  
  dtPlugin::dtPlugin() { 
  }

  dtPlugin::~dtPlugin() {
  }

  void dtPlugin::init(
    ::QDomElement const & element,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtCase const * const dC,
    lvH_dtPlugin const * const pL
	) {  
    //
    // set label of dtPlugin
    //
    labelHandling::setLabel( 
      dtXmlParserBase::getAttributeStr("label", element ) 
    );
    
    //
    // set options
    //
    optionHandling::init( element, bC, cV, aF, aG );
  }
  
  void dtPlugin::apply( void ) {
    dt__throwUnexpected(apply());
  }
       
  std::vector< std::string > dtPlugin::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }  
  
  void dtPlugin::setKernel( ::pugg::Kernel * kernel ) {
    _kernel.reset( kernel );
  }
  
}
