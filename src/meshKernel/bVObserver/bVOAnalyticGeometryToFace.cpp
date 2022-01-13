#include "bVOAnalyticGeometryToFace.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshFace.h>

#include <analyticGeometryHeaven/aGBuilder/uv_map2dTo3dClosestPointToPoint.h>

namespace dtOO {  
  bVOAnalyticGeometryToFace::bVOAnalyticGeometryToFace() {
  }

  bVOAnalyticGeometryToFace::~bVOAnalyticGeometryToFace() {
    
  }
  
  void bVOAnalyticGeometryToFace::bVOAnalyticGeometryToFace::init( 
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
    
//			<bVObserver 
//				name="bVOAnalyticGeometryToFace" 
//				analyticGeometry="
//					{aS_dt_hub_0}
//					{aS_ru_outlet_full}
//				"
//				increase_tolerance="1000."
//			/>								

    _inc 
    = 
    dtXmlParserBase::getAttributeFloatMuParse(
      "increase_tolerance", element, cV, aF, 1.
    );
    dt__forAllRefAuto(
      qtXmlBase::getAttributeStrVector("analyticGeometry", element),
      anEl
    ) {
      _m2d.push_back( map2dTo3d::MustConstDownCast(aG->get(anEl))->clone() );
    }
  }
  
  void bVOAnalyticGeometryToFace::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    //
    // set current model
    //
    ::GModel::setCurrent(gm);
    
    logContainer< bVOAnalyticGeometryToFace > logC(logINFO, "preUpdate()");    
    dt__forAllRefAuto(_m2d, aM2d) {
      logC() << aM2d.getLabel() << std::endl;
      
      //
      // local counter to prevent equal tags
      //
      int localCounter = 0;
      
      //
      // initialize min distance to max float
      //
      float minDistAv = std::numeric_limits<float>::max();
      dt__forAllRefAuto( gm->dtFaces(), aFace) {
        int inTol = 0;
        float distAv = 0.;
        dt__forAllRefAuto(aFace->dtVertices(), aVertex) {
          float dist 
          = 
          uv_map2dTo3dClosestPointToPoint(
            &(aM2d), 
            dtGmshModel::extractPosition(aVertex)
          ).distance();
          
          //
          // summerize distances
          //
          distAv = distAv + dist;
          
          //
          // increase if in precision
          //
          if ( analyticGeometry::inXYZTolerance( dist, _inc ) ) {
            inTol++;
          }
          // break if at least one point is not on the surface
          else {
            break;
          }
        }

        //
        // calculate average
        //
        distAv = distAv / aFace->vertices().size();
        
        //
        // store min distance
        //
        minDistAv = std::min(distAv, minDistAv);
        
        //
        // tag entity if in tolerance
        //
        if ( inTol == aFace->vertices().size() ) {
          std::string tagString(
            aM2d.getLabel()+"_"+stringPrimitive::intToString(localCounter)
          );
          logC() << "  > " << tagString << " ( " << distAv << " ) " << std::endl;
          gm->tagPhysical( aFace, tagString );
          localCounter++;
        }
      }
      logC() << "  > minDistAv = " << minDistAv << std::endl;      
    }
  }
}
