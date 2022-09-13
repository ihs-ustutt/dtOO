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