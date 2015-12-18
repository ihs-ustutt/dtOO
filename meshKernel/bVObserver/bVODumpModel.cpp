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

namespace dtOO {  
  bVODumpModel::bVODumpModel() {
  }

  bVODumpModel::~bVODumpModel() {
    
  }
  
  void bVODumpModel::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
		//
		// check size
		//		
    logContainer< bVODumpModel > logC(logINFO, "preUpdate()");
    dt__forFromToIndex(0, 4, dim) {
      dtGmshModel::intGEntityVMap map;
      gm->getPhysicalGroups(dim, map);
      dt__forAllRefAuto(map, aPair) {
        logC()
          << logMe::dtFormat(
            "Physical group: name = %s, dim = %d ( %d entities )"
          )
          % gm->getPhysicalName(dim, aPair.first) % dim % aPair.second.size()
          << std::endl;											        
      }
		}
  }
}
