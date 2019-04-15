#include "bVODumpModel.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
//#include <math.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <gmsh/Field.h>
#include <gmsh/Context.h>
#include <meshEngine/dtGmshVertex.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>

namespace dtOO {  
  bVODumpModel::bVODumpModel() {
  }

  bVODumpModel::~bVODumpModel() {
    
  }
  
  void bVODumpModel::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    //
		// set current model
		//
		::GModel::setCurrent( gm );
   
		//
		// check size
		//		
    logContainer< bVODumpModel > logC(logINFO, "preUpdate()");
    logC()
      << "number GVertex = " << gm->getNumVertices() << std::endl
      << "number GEdge = " << gm->getNumEdges() << std::endl
      << "number GFace = " << gm->getNumFaces() << std::endl
      << "number GRegion = " << gm->getNumRegions() << std::endl;
    dt__forFromToIndex(0, 4, dim) {
      dtGmshModel::intGEntityVMap map;
      gm->getPhysicalGroups(dim, map);
      dt__forAllRefAuto(map, aPair) {
        logC()
          << logMe::dtFormat(
            "Physical group ( %d ): name = %s, dim = %d ( %d entities )"
          )
          % aPair.first % gm->getPhysicalName(dim, aPair.first) 
          % dim % aPair.second.size()
          << std::endl;											        
      }
		}
    dt__forAllRefAuto(gm->vertices(), aV) {
      if (!aV) {
        logC() << "dtGmshVertex[] = NULL" << std::endl;
        continue;
      }
      logC() 
        << logMe::dtFormat("dtGmshVertex[ %3i ] = %s ( %x )")
          % aV->tag() % gm->getPhysicalString(aV) % aV
        << std::endl
        << "  edges = " << aV->edges() << std::endl;
    }        
    dt__forAllRefAuto(gm->edges(), aE) {
      if (!aE) {
        logC() << "dtGmshEdge[] = NULL" << std::endl;
        continue;
      }
      logC() 
        << logMe::dtFormat("dtGmshEdge[ %3i ] = %s ( %x )")
          % aE->tag() % gm->getPhysicalString(aE) % aE
        << std::endl
        << "  faces = " << aE->faces() << std::endl      
        << "  vertices = " << aE->vertices() << std::endl      
        << "  fullPhysicalList = [" << std::endl;
      dt__forAllRefAuto(gm->getFullPhysicalList(aE), aString) {
        logC() << aString << std::endl;
      }
      logC() << "]" << std::endl;              
    }    
    dt__forAllRefAuto(gm->faces(), aF) {
      if (!aF) {
        logC() << "dtGmshFace[] = NULL" << std::endl;
        continue;
      }
      logC() 
        << logMe::dtFormat("dtGmshFace[ %3i ] = %s ( %x )")
          % aF->tag() % gm->getPhysicalString(aF) % aF
        << std::endl
        << "  edges = " << aF->edges() << std::endl
        << "  orientations = " << aF->orientations() << std::endl
        << "  regions = " << aF->regions() << std::endl
        << "  fullPhysicalList = [" << std::endl;
      dt__forAllRefAuto(gm->getFullPhysicalList(aF), aString) {
        logC() << aString << std::endl;
      }
      logC() << "]" << std::endl;
    }
    dt__forAllRefAuto(gm->regions(), aR) {
      if (!aR) {
        logC() << "dtGmshRegion[] = NULL" << std::endl;
        continue;
      }      
      logC() 
        << logMe::dtFormat("dtGmshRegion[ %3i ] = %s ( %x )")
          % aR->tag() % gm->getPhysicalString(aR) % aR
        << std::endl
        << "  faceList = " << aR->faces() << std::endl
        << "  faceOrientations = " << aR->faceOrientations() << std::endl
        << "  fullPhysicalList = [" << std::endl;
      dt__forAllRefAuto(gm->getFullPhysicalList(aR), aString) {
        logC() << aString << std::endl;
      }
      logC() << "]" << std::endl;              
    }              
    logC()
      << "Gmsh fieldManager:" << std::endl
      << "size: " << gm->getFields()->size()
      << std::endl
      << dt__eval( CTX::instance()->mesh.lcFromPoints ) << std::endl
      << dt__eval( CTX::instance()->mesh.lcFromCurvature ) << std::endl
      << dt__eval( CTX::instance()->mesh.lcExtendFromBoundary ) << std::endl;
  }

  void bVODumpModel::postUpdate( void ) {
    preUpdate();
  }  
}
