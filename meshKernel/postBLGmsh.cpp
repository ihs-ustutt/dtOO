#include "postBLGmsh.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <dtXmlParserDecorator/qtXmlBase.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshMeshGFaceExtrude.h>
//#include "gmshBoundedVolume.h"

namespace dtOO {
	postBLGmsh::postBLGmsh() {
	}

	postBLGmsh::~postBLGmsh() {
	}
	
  void postBLGmsh::init( 
		QDomElement const & element,
    baseContainer const * const bC,					
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV
	) {
    //
    // init boundedVolume
    //
    boundedVolume::init(element, bC, cV, aF, aG, bV);
		
    _thickness = optionHandling::getOptionFloat("thickness");
    _intervals = optionHandling::getOptionInt("intervals");
    _bias = optionHandling::getOptionFloat("bias");
		
		//
		// boundedVolume
		//		
    QDomElement wElement = qtXmlPrimitive::getChild("boundedVolume", element);
    std::string label = qtXmlPrimitive::getAttributeStr("label", wElement);
		_faceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("faceLabel", wElement);
		_faceOrientation 
		= 
		qtXmlBase::getAttributeIntVectorMuParse("faceOrientation", wElement, cV, aF);
		
		//
		// get boundedVolume
		//
    _meshedBV = bV->get(label);
	}
	
  void postBLGmsh::makeGrid(void) {
		std::list< dtGmshFace const * > llF;
		dt__forAllConstIter(std::vector< std::string >, _faceLabel, it) {
		  llF.push_back( _meshedBV->getFace(*it) );
		}
		dtGmshMeshGFaceExtrude extruder(_thickness);
		extruder(llF, _faceOrientation);
		
		boundedVolume::setMeshed();
	}
  
	void postBLGmsh::makePreGrid(void) {
		boundedVolume::notify();
	}
  
	vectorHandling< renderInterface * > postBLGmsh::getRender( void ) const {
		return vectorHandling< renderInterface * >(0);
	}	
}
