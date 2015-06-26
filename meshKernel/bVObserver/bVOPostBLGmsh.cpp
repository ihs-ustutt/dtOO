#include "bVOPostBLGmsh.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <xmlHeaven/qtXmlBase.h>
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
		qtXmlBase::getAttributeIntVectorMuParse(
      "fixedFaceOrientation", element, cV, aF
    );
		_slidableFaceLabel 
		= 
		qtXmlPrimitive::getAttributeStrVector("slidableFaceLabel", element);
		_slidableFaceOrientation 
		= 
		qtXmlBase::getAttributeIntVectorMuParse(
      "slidableFaceOrientation", element, cV, aF
    );
		
		//
		// gmshBoundedVolume
		//
    dt__throwIf(gmshBoundedVolume::ConstDownCast(attachTo)==NULL, init());
  }
  
  void bVOPostBLGmsh::postUpdate( void ) {
		//
		// init faces and regions
		//
		std::list< dtGmshFace const * > faceList;
		std::vector< dtGmshRegion * > regVec;
		dt__forAllConstIter(std::vector< std::string >, _faceLabel, it) {
		  faceList.push_back( ptrBoundedVolume()->getFace(*it) );
			std::list< ::GRegion * > rr = faceList.back()->regions();
			dt__forAllConstIter(std::list< ::GRegion * >, rr, rIt) {
				dt__throwIf(!dtGmshRegion::DownCast(*rIt), postUpdate());
				regVec.push_back( dtGmshRegion::SecureCast(*rIt) );
			}
		}
		std::list< dtGmshFace const * > fixedFaceList;
		dt__forAllConstIter(std::vector< std::string >, _fixedFaceLabel, it) {
		  fixedFaceList.push_back( ptrBoundedVolume()->getFace(*it) );
			std::list< ::GRegion * > rr = fixedFaceList.back()->regions();
			dt__forAllConstIter(std::list< ::GRegion * >, rr, rIt) {
				dt__throwIf(!dtGmshRegion::DownCast(*rIt), postUpdate());
				regVec.push_back( dtGmshRegion::SecureCast(*rIt) );
			}			
		}	
		std::list< dtGmshFace const * > slidableFaceList;
		dt__forAllConstIter(std::vector< std::string >, _slidableFaceLabel, it) {
		  slidableFaceList.push_back( ptrBoundedVolume()->getFace(*it) );
			std::list< ::GRegion * > rr = slidableFaceList.back()->regions();
			dt__forAllConstIter(std::list< ::GRegion * >, rr, rIt) {
				dt__throwIf(!dtGmshRegion::DownCast(*rIt), postUpdate());
				regVec.push_back( dtGmshRegion::SecureCast(*rIt) );
			}			
		}	    
		
		//
		// get most frequent region
		//
		dtGmshRegion * commonReg = progHelper::mostFrequentChild(regVec);
		dt__info(
			postUpdate(), 
			<< "Adding elements to region with label " 
			<< ptrBoundedVolume()->getModel()->getPhysicalString(commonReg)
		);
				
		
		//
		// move boundary layer surfaces
		//
    std::vector< ::MVertex * > vv;
		std::vector< ::MElement * > ee;
		dtGmshMeshBoundaryLayer(
			faceList, _faceOrientation, 
			fixedFaceList, _fixedFaceOrientation,
      slidableFaceList, _slidableFaceOrientation,            
		  _thickness, _spacing, 
			_maxDihedralAngle, 
			_nSmoothingSteps, _nShrinkingSteps
		)(
		  ptrBoundedVolume()->getModel(), 
			vv, ee
		);
				
		//
		// create 3d mesh
		//
		if (!optionTrue("defer_mesh_3")) {
      ptrBoundedVolume()->getModel()->meshRegion();
    }
		
		//
		// add elements and vertices to commonReg
		//
		dt__forAllIter(std::vector< ::MVertex * >, vv, vIt) {
			(*vIt)->setEntity(commonReg);
			commonReg->addMeshVertex(*vIt);
		}
		dt__forAllIter(std::vector< ::MElement * >, ee, eIt) commonReg->addElement(*eIt);
  }
}
