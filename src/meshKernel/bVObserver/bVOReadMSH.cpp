#include "bVOReadMSH.h"

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVOReadMSH::bVOReadMSH() {
  }

  bVOReadMSH::~bVOReadMSH() {
    
  }
  
  void bVOReadMSH::bVOReadMSH::init( 
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
    
//		<bVObserver name="bVOReadMSH" 
//		  filename="mesh.msh"
//		/>									
		_filename = qtXmlBase::getAttributeStr("filename", element);		
  }
  
  void bVOReadMSH::preUpdate( void ) {
    dt__onlyMaster {
		  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());      
      
		  int status = gm->readMSH(_filename);
      
      dt__info(
        preUpdate(),
        << "Read MSH file = " << _filename << "." << std::endl
        << "Return status = " << status << "."
      );
      ptrBoundedVolume()->setMeshed();
    }
  }
}
