#include "bVOPostBLGmsh.h"

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
#include <meshEngine/dtGmshMeshBoundaryLayer.h>
#include <unstructured3dMesh.h>
#include "gmshBoundedVolume.h"

namespace dtOO {  
  bVOPostBLGmsh::bVOPostBLGmsh() {
  }

  bVOPostBLGmsh::~bVOPostBLGmsh() {
    
  }
  
  void bVOPostBLGmsh::bVOPostBLGmsh::init( 
		QDomElement const & element,
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
    
//		<bVObserver name="bVOPostBLGmsh"
//			spacing="{.5}"
//			thickness="1."
//			nSmoothingSteps="10"
//			nShrinkingSteps="10"
//			maxDihedralAngle="_pi*45./180."
//			faceLabel="{FACE_1}{FACE_2}"
//			faceOrientation="{-1}{1}"
//			fixedFaceLabel="{FACE_3}{FACE_4}"
//			fixedFaceOrientation="{1}{-1}"
//		/>		
							
		_spacing 
		= 
		qtXmlBase::getAttributeFloatVectorMuParse("spacing", element, cV, aF);		
    _thickness 
		= 
		qtXmlBase::getAttributeFloatMuParse("thickness", element, cV, aF);
		_nSmoothingSteps 
		= 
		qtXmlBase::getAttributeIntMuParse("nSmoothingSteps", element, cV, aF);		
		_nShrinkingSteps 
		=
		qtXmlBase::getAttributeIntMuParse("nShrinkingSteps", element, cV, aF);		
		_maxDihedralAngle 
		= 
		qtXmlBase::getAttributeFloatMuParse("maxDihedralAngle", element, cV, aF);		
		
		//
		// boundedVolume
		//		
		_faceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("faceLabel", element);
		_faceOrientation 
		= 
		qtXmlBase::getAttributeIntVectorMuParse("faceOrientation", element, cV, aF);
		_fixedFaceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("fixedFaceLabel", element);
		_fixedFaceOrientation 
		= 
		qtXmlBase::getAttributeIntVectorMuParse("fixedFaceOrientation", element, cV, aF);
		
		//
		// gmshBoundedVolume
		//
    dt__throwIf(gmshBoundedVolume::ConstDownCast(attachTo)==NULL, init());
  }
  
  void bVOPostBLGmsh::postUpdate( void ) {
		std::list< dtGmshFace const * > faceList;
		dt__forAllConstIter(std::vector< std::string >, _faceLabel, it) {
		  faceList.push_back( ptrBoundedVolume()->getFace(*it) );
		}
		std::list< dtGmshFace const * > fixedFaceList;
		dt__forAllConstIter(std::vector< std::string >, _fixedFaceLabel, it) {
		  fixedFaceList.push_back( ptrBoundedVolume()->getFace(*it) );
		}		
		//
		// create new region, tag it and add it to the model
		//
		_dtR 
		= 
		new dtGmshRegion(
			ptrBoundedVolume()->getModel(), 
			ptrBoundedVolume()->getModel()->getMaxRegionTag()+1
		);
		ptrBoundedVolume()->getModel()->tagPhysical(_dtR, ptrBoundedVolume()->getLabel());
		ptrBoundedVolume()->getModel()->add(_dtR);
		
		dtGmshMeshBoundaryLayer(
		  _thickness, _spacing, 
			_maxDihedralAngle, 
			_nSmoothingSteps, _nShrinkingSteps
		)(
		  _dtR, 
			faceList, _faceOrientation, 
			fixedFaceList, _fixedFaceOrientation
		);
				
    _dtR->_status = ::GEntity::MeshGenerationStatus::DONE;
		
		ptrBoundedVolume()->getModel()->meshRegion();
  }
}
