#include "mesherOpt.h"
#include <logMe/logMe.h>
#include <boundedVolume.h>
#include <characterizable.h>
#include <limits>

namespace dtOO {
  mesherOpt::mesherOpt(int argc, char *argv[] ) : mesher(argc, argv) {
    _pOut_optiControlP = addOutputPort("_pOut_optiControlP", "USR_Optim", "_pOut_optiControlPDescription");
  }
  
  int mesherOpt::compute(char const * str) {
    try {    

//      FILELog::ReportingLevel() = logERROR;
      int retVal = mesher::compute(str);
//      FILELog().ReportingLevel() = logDEBUG;
      
//      FILELog().ReportingLevel() = logERROR;
      if ( _pOut_optiControlP->isConnected() ) {
        //
        // check for optimization
        //
        characterizable * cP;
        vectorHandling< boundedVolume * > & bVRef = getRefToBoundedVolume();
        dt__FORALL(bVRef, ii,
          dt__CANDOWNCAST(bVRef[ii], characterizable, cP);
          if (cP) {
            //
            // write to file
            //
            cP->characterize( bVRef[ii] );
          }
        );
        if (cP) cP->sendCanRead();
        
        return STOP_PIPELINE;        
      }
      
      return retVal;
    }
    catch (eGeneral & eGenRef) {
      dt__catch(compute(), eGenRef.what());
      return FAIL;
    }     
  }

  mesherOpt::~mesherOpt() {
    
  }  
}
MODULE_MAIN(designTool, dtOO::mesherOpt)