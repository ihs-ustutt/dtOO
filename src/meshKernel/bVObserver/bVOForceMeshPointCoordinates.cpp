#include "bVOForceMeshPointCoordinates.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <baseContainerHeaven/baseContainer.h>
#include <dtTransformerHeaven/dtTransformer.h>

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/vec3dThreeD.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>

namespace dtOO {  
  bVOForceMeshPointCoordinates::bVOForceMeshPointCoordinates() {
  }

  bVOForceMeshPointCoordinates::~bVOForceMeshPointCoordinates() {
    
  }
  
  void bVOForceMeshPointCoordinates::bVOForceMeshPointCoordinates::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV,
		boundedVolume * attachTo
  ) {
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("coordinateFunction", element)
      ||
      !dtXmlParserBase::hasAttribute("faceLabel", element),
      init()
    );
    
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
    //    <bVObserver 
    //      name="bVOForceMeshPointCoordinates" 
    //      coordinateFunction="f_forceNy"
    //      faceLabel="INGVRUDTOUT_SYMNY"
    //    />
			
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStrVector("faceLabel", element);
    
    _v3d.reset(
      vec3dThreeD::MustConstDownCast(
        aF->get( 
          dtXmlParserBase::getAttributeStr("coordinateFunction", element) 
        )
      )->clone()
    );
  }
  
  void bVOForceMeshPointCoordinates::postUpdate( void ) {
    dt__throwIf( !ptrBoundedVolume()->isMeshed(), postUpdate() );

		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
    dt__forAllRefAuto(_faceLabel, aLabel) {
      std::list< dtGmshFace * > theFaces 
      = 
      gm->getDtGmshFaceListByPhysical( aLabel );
      
      dt__forAllRefAuto(theFaces, aFace) {
        
        dt__forFromToIndex(0, aFace->getNumMeshElements(), ii) {
          std::vector< ::MVertex * > verts;
          aFace->getMeshElement(ii)->getVertices( verts );
          dt__forAllRefAuto(verts, aVert) {
            dtPoint3 xyz 
            = 
            _v3d->YdtPoint3( aVert->x(), aVert->y(), aVert->z() );
            
            aVert->setXYZ( xyz.x(), xyz.y(), xyz.z() );
          }
        }
      }
      
    }
  }
}
