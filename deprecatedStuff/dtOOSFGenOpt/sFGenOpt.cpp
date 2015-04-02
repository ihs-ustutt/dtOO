#include "sFGenOpt.h"
#include <logMe/logMe.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <characterizable.h>


namespace dtOO {
  sFGenOpt::sFGenOpt(int argc, char *argv[] ) : sFGen(argc, argv) {
    _pOut_optiControlP = addOutputPort("_pOut_optiControlP", "USR_Optim", "_pOut_optiControlPDescription");
  }
  
  int sFGenOpt::compute(char const * str) {
    try {    
      int retVal = sFGen::compute(str);

      if ( _pOut_optiControlP->isConnected() ) {        
        //
        // check for optimization
        //
        characterizable * cP;
        coDoSetHandling< scaFunction * > & sFRef = getRefToScaFunction();
        dt__FORALL(sFRef, ii,
          dt__CANDOWNCAST(sFRef[ii], characterizable, cP);

        
          if (cP) {
            //
            // write to file
            //
            cP->characterize( sFRef[ii] );
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

  sFGenOpt::~sFGenOpt() {
    
  }  
}
MODULE_MAIN(designTool, dtOO::sFGenOpt)