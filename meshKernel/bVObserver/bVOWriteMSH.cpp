#include "bVOWriteMSH.h"

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVOWriteMSH::bVOWriteMSH() {
  }

  bVOWriteMSH::~bVOWriteMSH() {
    
  }
  
  void bVOWriteMSH::bVOWriteMSH::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		boundedVolume * attachTo
  ) {		
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
//		<bVObserver name="bVOWriteMSH" 
//		  filename="mesh.msh"
//		  saveAll="false"
//		/>									
		_filename 
		= 
		qtXmlBase::getAttributeStr("filename", element);		
    
    _saveAll = false;
    if (qtXmlBase::hasAttribute("saveAll", element)) {
      _saveAll = qtXmlBase::getAttributeBool("saveAll", element);
    }
  }
  
  void bVOWriteMSH::postUpdate( void ) {
    dt__onlyMaster {
		  ptrBoundedVolume()->getModel()->writeMSH(_filename, 2.2, false, _saveAll);
    }
  }
}
