#include "ruOpti.h"

#include <constValueHeaven/constValue.h>
#include <api/coCallFeedback.h>

namespace dtOO {
  ruOpti::ruOpti(int argc, char *argv[]) : coModule(argc, argv, "ruOpti") {
    _pIn_constValueP = addInputPort("inConstValueP", "Vec3", "inConstValueDescription");
    _pIn_resValueP = addInputPort("resValueP", "Vec3", "resValueDescription");   
    _p_xmlFilebrowser = addFileBrowserParam("xmlFilebrowser", "xmlFilebrowserDescrition");
    _p_xmlFilebrowser->setValue(".", "*.xml/*");

    _exCounter = 0;
  }

  void ruOpti::param(const char* paramName, bool inMapLoading) {
  }
  
  ruOpti::~ruOpti() {
  }
  
  int ruOpti::compute(const char * port) {
    //
    // init log file
    //
    std::string logFileName = std::string( coModule::getTitle() );
    abstractModule::initializeLogFile( "./"+logFileName+".log" );

    //
    // recreate constValues
    //
    covise::coDistributedObject const * inObj = _pIn_constValueP->getCurrentObject();
    covise::coDoSet const * set = dynamic_cast< covise::coDoSet const * >(inObj);
    _cVal.destroy();
    abstractModule::recreateConstValues(set, _cVal);
    
    coCallFeedback cCF;
    cCF.init(
          "INTERACTOR", 
          _pIn_constValueP->getCurrentObject()->getAttribute("INTERACTOR")
    );
    
      if (_exCounter <= 9) {
        cCF.setChoiceParam("constValueChoiceParam", "cV_cGrid_11");
        cCF.setFloatSliderParam( "constValueFloatSliderParam", (float) _exCounter / 10. );
        _exCounter++;
        std::cout << "_exCounter = " << _exCounter << std::endl;
        cCF.executeModule();
      }
  }
}
MODULE_MAIN(designTool, dtOO::ruOpti)