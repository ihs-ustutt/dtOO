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
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result  
	) const {
    dt__info(buildPartCompound(), << "Call on aGXmlBuilder");		
	}

  std::vector< std::string > aGXmlBuilder::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }  
}