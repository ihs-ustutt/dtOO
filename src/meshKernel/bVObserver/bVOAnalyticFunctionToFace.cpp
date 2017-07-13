#include "bVOAnalyticFunctionToFace.h"

#include <logMe/logMe.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaThreeD.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshFace.h>

//#include <analyticGeometryHeaven/aGBuilder/uv_map2dTo3dClosestPointToPoint.h>

namespace dtOO {  
  bVOAnalyticFunctionToFace::bVOAnalyticFunctionToFace() {
  }

  bVOAnalyticFunctionToFace::~bVOAnalyticFunctionToFace() {
    
  }
  
  void bVOAnalyticFunctionToFace::bVOAnalyticFunctionToFace::init( 
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
    
    //  	<bVObserver 
    //      name="bVOAnalyticFunctionToFace"
    //			analyticFunction="aS_sym_nx_analyticFunction"
    //    />		    	    					

    dt__forAllRefAuto(
      qtXmlBase::getAttributeStrVector("analyticFunction", element),
      anEl
    ) {
      _sca.push_back( scaThreeD::MustConstDownCast(aF->get(anEl))->clone() );
    }
  }
  
  void bVOAnalyticFunctionToFace::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    //
    // set current model
    //
    ::GModel::setCurrent(gm);
    
    logContainer< bVOAnalyticFunctionToFace > logC(logINFO, "preUpdate()");    
    dt__forAllRefAuto(_sca, aSca) {
      logC() << aSca.getLabel() << std::endl;
      
      //
      // local counter to prevent equal tags
      //
      int localCounter = 0;
      
      dt__forAllRefAuto( gm->dtFaces(), aFace) {
        int inTol = 0;
        dt__forAllRefAuto(aFace->dtVertices(), aVertex) {
          //
          // ask function if vertex is part of the face to tag
          //
          if ( aSca.YFloat( aVertex->x(), aVertex->y(), aVertex->z() ) != 0. ) {
            inTol++;
          }
        }

        //
        // tag entity if all vertices of face are part of the function
        //
        if ( inTol == aFace->vertices().size() ) {
          std::string tagString(
            aSca.getLabel()+"_"+stringPrimitive::intToString(localCounter)
          );
          logC() << "  > " << tagString  << std::endl;
          gm->tagPhysical( aFace, tagString );
          localCounter++;
        }
      }
    }
  }
}
