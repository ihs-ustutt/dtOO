#include "abstractModule.h"
#include <logMe/logMe.h>

namespace dtOO { 
  abstractModule::abstractModule() {
  }

  abstractModule::~abstractModule() {
  }

  void abstractModule::initializeLogFile( std::string const logFileName )  {
		logMe::initLog(logFileName);
  }    
  
  void abstractModule::closeLogFile( void ) {
		logMe::closeLog();
  }
  
  void abstractModule::updateChoiceParam(
         covise::coChoiceParam * choiceParamP, 
         std::vector< std::string > * vecHP
       ) {
    
    int label = 0;
    std::string labelStr = "";
    if (choiceParamP->getNumChoices() > 0) {
      label = choiceParamP->getValue();
      labelStr = choiceParamP->getActLabel();
    }
    
    std::vector<string> choicesFun(vecHP->size(), "empty");
    for (int ii = 0;ii<vecHP->size();ii++) {
      choicesFun[ii] = vecHP->at(ii).c_str();
    }
    choiceParamP->disable();
    choiceParamP->setValue(vecHP->size(), choicesFun, 0);
    choiceParamP->enable();

    if (choiceParamP->getNumChoices() > 0) {    
      if ( !(choiceParamP->getLabel(label) == labelStr) ) {
        label = 0;
      }
    }
    choiceParamP->setValue(label);
  }  
}
