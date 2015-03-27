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
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshMeshGFaceExtrude.h>
#include <unstructured3dMesh.h>
#include "gmshBoundedVolume.h"

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
		
    _thickness = optionHandling::getOptionFloat("thickness", cV, aF);
    _intervals = optionHandling::getOptionInt("intervals", cV, aF);
    _bias = optionHandling::getOptionFloat("bias", cV, aF);
		_nSmoothingSteps = optionHandling::getOptionInt("nSmoothingSteps", cV, aF);		
		_maxDihedralAngle = optionHandling::getOptionInt("maxDihedralAngle", cV, aF);		
		
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
		_fixedFaceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("fixedFaceLabel", wElement);
		_fixedFaceOrientation 
		= 
		qtXmlBase::getAttributeIntVectorMuParse("fixedFaceOrientation", wElement, cV, aF);
		
		//
		// get boundedVolume
		//
    dt__ptrAss( _meshedBV, gmshBoundedVolume::ConstDownCast(bV->get(label)) );
	}
	
  void postBLGmsh::makeGrid(void) {
		std::list< dtGmshFace const * > faceList;
		dt__forAllConstIter(std::vector< std::string >, _faceLabel, it) {
		  faceList.push_back( _meshedBV->getFace(*it) );
		}
		std::list< dtGmshFace const * > fixedFaceList;
		dt__forAllConstIter(std::vector< std::string >, _fixedFaceLabel, it) {
		  fixedFaceList.push_back( _meshedBV->getFace(*it) );
		}		
		_dtR 
		= 
		new dtGmshRegion(_meshedBV->getModel(), _meshedBV->getModel()->getMaxRegionTag()+1);
		_meshedBV->getModel()->tagPhysical(_dtR, getLabel());
		_meshedBV->getModel()->add(_dtR);
		dtGmshMeshGFaceExtrude extruder(_thickness, _maxDihedralAngle, _nSmoothingSteps);
		extruder(
		  _dtR, 
			faceList, _faceOrientation, 
			fixedFaceList, _fixedFaceOrientation
		);
		
		_meshedBV->getModel()->writeMSH("postBLGmsh.msh");
		
		//
		// update physicals because we add a new region
		//
		_meshedBV->updatePhysicals();
		
		boundedVolume::setMeshed();
	}
  
	void postBLGmsh::makePreGrid(void) {
		boundedVolume::notify();
	}
  
	vectorHandling< renderInterface * > postBLGmsh::getRender( void ) const {
	  vectorHandling< renderInterface * > rV;
				
		std::vector< ::MElement const * > elThreeD;
		for (int jj=0;jj<_dtR->getNumMeshElements(); jj++) {
			elThreeD.push_back( _dtR->getMeshElement(jj) );	
		}							
		rV.push_back(dtGmshModel::toUnstructured3dMesh(elThreeD));	
		
		return rV;
	}	
}
