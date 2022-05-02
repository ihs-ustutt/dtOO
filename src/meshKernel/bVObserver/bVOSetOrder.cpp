#include "bVOSetOrder.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>

#include <gmsh.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOSetOrder::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOSetOrder, new bVOSetOrder())
  );
  
  bVOSetOrder::bVOSetOrder() {
  }

  bVOSetOrder::~bVOSetOrder() {
    
  }
  
  void bVOSetOrder::bVOSetOrder::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV,
		boundedVolume * attachTo
  ) {
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_order
		= 
		dtXmlParserBase::getAttributeIntMuParse(
      "order", element, cV, aF
    );
  }
  
  void bVOSetOrder::postUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );
		dt__throwIf( !ptrBoundedVolume()->isMeshed(), postUpdate() );
    
    ::gmsh::model::mesh::setOrder( _order );
  }
}
