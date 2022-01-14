#include "bVOSetPrescribedMeshSizeAtPoints.h"

#include <logMe/logMe.h>
#include <logMe/logContainer.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <math.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVOSetPrescribedMeshSizeAtPoints::bVOSetPrescribedMeshSizeAtPoints() {
  }

  bVOSetPrescribedMeshSizeAtPoints::~bVOSetPrescribedMeshSizeAtPoints() {
    
  }
  
  void bVOSetPrescribedMeshSizeAtPoints::bVOSetPrescribedMeshSizeAtPoints::init( 
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
    
		// <bVObserver 
		//   name="bVOSetPrescribedMeshSizeAtPoints" 
		//   meshSize="0.1"
		// />
								
    if ( dtXmlParserBase::isAttributeVector("meshSize", element) ) {
      _meshSize
      = 
      dtXmlParserBase::getAttributeFloatVectorMuParse("meshSize", element, cV, aF);
    }
    else {
      _meshSize
      = 
      std::vector< dtReal >( 
        1, 
        dtXmlParserBase::getAttributeFloatMuParse("meshSize", element, cV, aF) 
      );
    }
  }
  
  void bVOSetPrescribedMeshSizeAtPoints::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
			
    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    if (_meshSize.size() == 1) {
      _meshSize
      = 
      std::vector< dtReal >( gm->getNumVertices(), _meshSize[0] );
    }
    dt__throwIf(
      gm->getNumVertices()!=_meshSize.size(), 
      preUpdate()
    );
    
    int cc = 0;
    dt__forAllRefAuto(gm->vertices(), aV) {
      aV->setPrescribedMeshSizeAtVertex(_meshSize[cc]);
      cc = cc + 1;
    }

    //
    // output
    //
    logContainer< bVOSetPrescribedMeshSizeAtPoints > logC(logINFO, "preUpdate()");
    dt__forAllRefAuto(gm->vertices(), aV) {
      logC() 
        << logMe::dtFormat("dtGmshVertex[ %3i ]: meshSize = %f")
          % aV->tag() % aV->prescribedMeshSizeAtVertex() 
        << std::endl;
    }        
  }
}
