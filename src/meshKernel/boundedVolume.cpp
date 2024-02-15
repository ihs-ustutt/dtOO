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

#include "boundedVolume.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <bVObserver/bVOInterfaceFactory.h>
#include <bVObserver/bVOInterface.h>
#include <xmlHeaven/qtXmlPrimitive.h>
#include <meshEngine/dtGmshFace.h>


namespace dtOO {  
  boundedVolume::boundedVolume() { 
		_meshed = false;
    _preMeshed = false;
  }

  boundedVolume::~boundedVolume() {
  }

  std::vector< std::string > boundedVolume::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }    
 
  void boundedVolume::jInit( 
    jsonPrimitive const & jE,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV
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
    // check for bVObservers
    //
    
    dt__forAllRefAuto(
      _config.lookupDef< std::vector< jsonPrimitive > >(
        "bVObserver", 
        std::vector< jsonPrimitive >() 
      ), 
      anObs
    ) {
      bVOInterface * bVI
      =
      bVOInterfaceFactory::create( anObs.lookup< std::string >("name") );
      bVI->jInit(anObs, bC, cV, aF, aG, bV, this);
      this->attachBVObserver( bVI );
    }

		dt__debug( jInit(), << "config() = " << _config.toStdString() );
  }

  void boundedVolume::init( 
		::QDomElement const & element,
		baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV
	) {
    //
    // set label of boundenVolume
    //
    labelHandling::setLabel( qtXmlPrimitive::getAttributeStr("label", element ) );
    
    //
    // set options
    //
    optionHandling::init( element, bC, cV, aF, aG );
    
    //
    // check for bVObservers
    //
    if ( qtXmlPrimitive::hasChild("bVObserver", element) ) {
      std::vector< ::QDomElement > transElement 
			= 
			qtXmlPrimitive::getChildVector("bVObserver", element);
      
      for (int ii=0; ii<transElement.size(); ii++) {
        bVOInterface * bVI
        =
        bVOInterfaceFactory::create(
					qtXmlPrimitive::getAttributeStr("name", transElement[ii])
				);
        bVI->init(transElement[ii], bC, cV, aF, aG, bV, this);
        this->attachBVObserver( bVI );
      }
    }
  }

  bool boundedVolume::isMeshed( void ) const {
		return _meshed;
	}
	
  void boundedVolume::setMeshed( void ) {
    dt__info(setMeshed(), << "Set " << getLabel() << " -> meshed.");
		_meshed = true;
	}

  bool boundedVolume::isPreMeshed( void ) const {
		return _preMeshed;
	}
	
  void boundedVolume::setPreMeshed( void ) {
    dt__info(setPreMeshed(), << "Set " << getLabel() << " -> preMeshed.");
		_preMeshed = true;
	} 

  jsonPrimitive & boundedVolume::config( void ) {
    return _config;
  }
  
  jsonPrimitive const & boundedVolume::config( void ) const {
    return _config;
  }
}
