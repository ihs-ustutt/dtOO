#include "boundedVolume.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <bVObserver/bVOInterfaceFactory.h>
#include <bVObserver/bVOInterface.h>
#include <dtXmlParserDecorator/qtXmlPrimitive.h>
#include <meshEngine/dtGmshFace.h>


namespace dtOO {  
  boundedVolume::boundedVolume() { 
		_meshed = false;
  }

  boundedVolume::~boundedVolume() {
  }

  void boundedVolume::init( 
		QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV
	) {
    //
    // set label of boundenVolume
    //
    labelHandling::setLabel( qtXmlPrimitive::getAttributeStr("label", element ) );
    
    //
    // set options
    //
    optionHandling::init( &element );
    
    //
    // check for bVObservers
    //
    if ( qtXmlPrimitive::hasChild("bVObserver", element) ) {      
      std::vector< QDomElement > transElement 
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
		_meshed = true;
	}
	
	std::vector< std::string > boundedVolume::getMeshTags( void ) const {
		return std::vector< std::string >(0);
	}
	
	dtGmshFace const * boundedVolume::getFace( std::string const & tag ) const {
		return NULL;
	}

	dtGmshModel * boundedVolume::getModel( void ) const {
		return NULL;
	}
}


