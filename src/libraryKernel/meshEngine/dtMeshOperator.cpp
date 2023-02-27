#include "dtMeshOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
  dtMeshOperator::dtMeshOperator() {
  }

  dtMeshOperator::~dtMeshOperator() {
  }

  std::vector< std::string > dtMeshOperator::factoryAlias( void ) const {
    return std::vector< std::string>(0);
  }    

  void dtMeshOperator::jInit(
    jsonPrimitive const & jE,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtMeshOperator const * const mO
  ) {
    _config = jE;
    dt__debug( jInit(), << _config.toStdString() );
    labelHandling::jInit(_config);
    optionHandling::jInit(_config);
  }

  void dtMeshOperator::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtMeshOperator const * const mO    
  ) {
    labelHandling::setLabel(
      qtXmlPrimitive::getAttributeStr("label", element ) 
    );
    optionHandling::init( element, bC, cV, aF, aG );
  }

  jsonPrimitive & dtMeshOperator::config( void ) {
    return _config;
  }
  
  jsonPrimitive const & dtMeshOperator::config( void ) const {
    return _config;
  }
}
