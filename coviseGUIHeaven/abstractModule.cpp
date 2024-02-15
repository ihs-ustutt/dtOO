/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "abstractModule.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
namespace dtOO { 
  abstractModule::abstractModule() {
  }

  abstractModule::~abstractModule() {
  }

  std::string abstractModule::initializeLogFile( std::string const logFileName )  {
		return logMe::initLog(logFileName);
  }    
  
  void abstractModule::closeLogFile( void ) {
		logMe::closeLog();
  }
  
  void abstractModule::updateChoiceParam(
    covise::coChoiceParam * choiceParamP, std::vector< std::string > const * const vecHP
  ) {
		if (vecHP->size() == 0) {
			choiceParamP->disable();
			return;
		}
			
    int label = 0;
    std::string labelStr = "";
    if (choiceParamP->getNumChoices() > 0) {
      label = choiceParamP->getValue();
      labelStr = choiceParamP->getActLabel();
    }
    
    std::vector<std::string> choicesFun(vecHP->size(), "empty");
    for (int ii = 0;ii<vecHP->size();ii++) {
      choicesFun[ii] = vecHP->at(ii);
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

  void abstractModule::updateChoiceParam(
    covise::coChoiceParam * choiceParamP, 
    std::vector< std::string > const & vecHP
  ) {
    updateChoiceParam(choiceParamP, &vecHP);  
  }
      
	std::string abstractModule::blankReConvert(std::string const & str) {
		return stringPrimitive::replaceStringInString("\177", " ", str);
	}   	
}
