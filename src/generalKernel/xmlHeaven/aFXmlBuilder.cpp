#include "aFXmlBuilder.h"
#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>

namespace dtOO {
  aFXmlBuilder::aFXmlBuilder() {
  }

  void aFXmlBuilder::buildPartCompound(
    ::QDomElement const & toBuild, 
    baseContainer * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aFPtrVec * result
	) const {
    dt__info(buildPartCompound(), << "Call on aFXmlBuilder");
  }
	
  aFXmlBuilder::~aFXmlBuilder() {
  }
  
  std::vector< std::string > aFXmlBuilder::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }
}