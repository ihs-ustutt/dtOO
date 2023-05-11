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
  
  void bVOSetOrder::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV,
		boundedVolume * attachTo
  ) {
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
 	  jsonPrimitive jE;
    jE.append< dtInt >(
      "_order",
      dtXmlParserBase::getAttributeIntMuParse(
        "order", element, cV, aF
      )
    );
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
  }
  
  void bVOSetOrder::postUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );
		dt__throwIf( !ptrBoundedVolume()->isMeshed(), postUpdate() );
    
    ::gmsh::model::mesh::setOrder( config().lookup< dtInt >("_order" ) );
  }
}
