#include "bVOSetPrescribedMeshSizeAtPoints.h"

#include <logMe/logMe.h>
#include <logMe/logContainer.h>
#include <progHelper.h>
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
    
		// <bVObserver 
		//   name="bVOSetPrescribedMeshSizeAtPoints" 
		//   meshSize="0.1"
		// />
								
		_meshSize
		= 
		dtXmlParserBase::getAttributeFloatMuParse("meshSize", element, cV, aF);
  }
  
  void bVOSetPrescribedMeshSizeAtPoints::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
			
    dt__forAllRefAuto(gm->vertices(), aV) {
      aV->setPrescribedMeshSizeAtVertex(_meshSize);
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
