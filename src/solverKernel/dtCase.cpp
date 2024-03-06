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

#include "dtCase.h"

#include <logMe/logMe.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <interfaceHeaven/systemHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include <interfaceHeaven/stringPrimitive.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace dtOO {
	dtCase::dtCase() {
	}

	dtCase::~dtCase() {
	}
  
  std::vector< std::string > dtCase::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }  
  
  void dtCase::jInit( 
    jsonPrimitive const & jE,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtCase const * const dC
  ) {
    _config = jE;
    //
    // set label of boundenVolume
    //
    labelHandling::jInit(jE);
    //
    // set options
    //
    optionHandling::jInit(jE);
		//
    // debug output
    //
    dt__debug( jInit(), << "config() = " << _config.toStdString() );
  }


  void dtCase::init(
	  ::QDomElement const & element,
		baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtCase const * const dC
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

  void dtCase::runCurrentState( void ) {
    dt__throwUnexpected(runCurrentState());
  }
  
  std::string dtCase::createDirectory( std::string const & state ) const {
    std::string wDir 
    = 
    getDirectory( state );
    
    if ( !systemHandling::directoryExists(wDir) ) {
      systemHandling::createDirectory(wDir);
    }
    
    dt__info( createDirectory(), << dt__eval(wDir) );    

    return wDir;
  }  
  
  std::string dtCase::getDirectory( std::string const & state ) const {
    std::string wDir 
    = 
    staticPropertiesHandler::getInstance()->getOption("workingDirectory")
    +
    "/"
    +
    getLabel();

    //
    // extend directory if state is not empty
    //
    if (state != "") wDir = wDir+"_"+state;

    return wDir;
  }
  
  jsonPrimitive & dtCase::config( void ) {
    return _config;
  }
  
  jsonPrimitive const & dtCase::config( void ) const {
    return _config;
  }
}
