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
}