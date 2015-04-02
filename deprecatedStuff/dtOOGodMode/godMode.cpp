#include "godMode.h"
#include <dtXmlParserDecorator/dtXmlParser.h>
#include <debugHeaven/extInfoContainer.h>
#include <do/coDoSet.h>
#include <logMe/logMe.h>
#include <analyticFunctionHeaven/scaFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>

#define SUCCESS 1;
#define FAILURE 0;

namespace dtOO { 
  godMode::godMode(int argc, char *argv[] ) : coModule(argc, argv, "godMode") {
    _pIn_extInfoContainerP = addInputPort("_pIn_extInfoContainerP", "USR_ExtInfo", "_pIn_extInfoContainerPDescription");
    _pIn_extInfoContainerP->setRequired(1);    
    _pOut_sFunP = addOutputPort("_pOut_sFunP", "USR_ScaFun", "_pOut_sFunPDescription");
    _pOut_aGeoP = addOutputPort("_pOut_aGeoP", "USR_AnGeo", "_pOut_aGeoPDescription");
  }
  
  int godMode::compute(char const * str) {
    try {    
      //
      // init log file
      //
      std::string logFileName = std::string( coModule::getTitle() );
      abstractModule::initializeLogFile( "./"+logFileName+".log" );
    
      //
      // recreate extInfoContainer
      //
      covise::coDistributedObject const * inObj = _pIn_extInfoContainerP->getCurrentObject();
      covise::coDoSet const * set = dynamic_cast< covise::coDoSet const * >(inObj);
      if ( set == NULL) {
        dt__debug(compute(),
                << dt__eval(set) );
        return SUCCESS;
      }

        extInfoContainer eIC;
        eIC.unpackFromCoDoSet( set );
        
      
      //
      // send to next module
      //
      if ( _pOut_sFunP->isConnected() ) {
        if (eIC.getNumScaFunctions() > 0) {
          _pOut_sFunP->setCurrentObject(
                         eIC.getRefToScaFunction().packToCoDoSet( 
                           eIC.getRefToScaFunction(),
                           _pOut_sFunP->getObjName()
                         )
                       );
        }
      }
      if ( _pOut_aGeoP->isConnected() ) {
        if (eIC.getNumAnGeos() > 0) {
          _pOut_aGeoP->setCurrentObject(
                         eIC.getRefToAnGeo().packToCoDoSet( 
                           eIC.getRefToAnGeo(),
                           _pOut_aGeoP->getObjName()
                         )
                       );
        }
      }        
      
//      //
//      // destroy temporary function container
//      //
//      tmpSFun.destroy();
      

//      //
//      // functions
//      //
//      if (_pOut_functionP->isConnected()) {
//        if (_sFun.size() != 0) {
//          char const * label = _p_functionChoiceParam->getActLabel();
//          scaFunction * sF = _sFun.get( label );
//          covise::coDoSet * set = sF->toCoDoSet( _pOut_functionP->getObjName() );
//          _pOut_functionP->setCurrentObject( set );
//        }
//      }    
//      if (_pOut_functionSetP->isConnected()) {
//        if (_sFun.size() != 0) {
//          int choice = _p_functionSetChoiceParam->getValue();
//          covise::coDoSet * set = _sFunSet[choice].toCoDoSet(
//                                    _sFunSet[choice], 
//                                    _pOut_functionSetP->getObjName() 
//                                  );
//          _pOut_functionSetP->setCurrentObject( set );
//        }
//      }          
      return SUCCESS;
    }
    catch (eGeneral & eGenRef) {
      dt__catch(compute(), eGenRef.what());
      send_stop_pipeline();
      return FAILURE;
    }    
          
  }
  
  godMode::~godMode() {
    
  }  
}
MODULE_MAIN(designTool, dtOO::godMode)