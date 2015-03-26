#include "postBLGmsh.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <dtXmlParserDecorator/qtXmlBase.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
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
		_nSmoothingSteps = optionHandling::getOptionInt("nSmoothingSteps");		
		
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
		_regionLabel
		= 
		qtXmlPrimitive::getAttributeStr("regionLabel", wElement);
		
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
		dtGmshMeshGFaceExtrude extruder(_thickness, _nSmoothingSteps);
		extruder(_meshedBV->getRegion(_regionLabel), llF, _faceOrientation);
		
		_meshedBV->getModel()->writeMSH("postBLGmsh.msh");
		boundedVolume::setMeshed();
	}
  
	void postBLGmsh::makePreGrid(void) {
		boundedVolume::notify();
	}
  
	vectorHandling< renderInterface * > postBLGmsh::getRender( void ) const {
		return vectorHandling< renderInterface * >(0);
	}	
}
