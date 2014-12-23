#ifndef ABSTRACTMODULE_H
#define	ABSTRACTMODULE_H


#include <string.h>
#include <api/coChoiceParam.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <constValueHeaven/constValue.h>
#include <functionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>

namespace dtOO {  
  class abstractModule {    
  public:
    dt__CLASSNAME(abstractModule);
    virtual ~abstractModule();
    static std::string initializeLogFile( std::string const logFileName ) ; 
    static void closeLogFile( void );
    static void updateChoiceParam(
         covise::coChoiceParam * choiceParamP, 
         std::vector< std::string > * vecHP
       );    
    template< class T >
    static void updateChoiceParam(
      covise::coChoiceParam * choiceParamP, 
      vectorHandling< T > * vecHP
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

      std::vector<string> choicesFun(vecHP->size(), "empty");
      for (int ii = 0;ii<vecHP->size();ii++) {
        if ( !vecHP->at(ii)->Is( labelHandling() ) ) {
          dt__THROW(
            updateChoiceParam(), 
            << "vecHP< T >: T is not a baseclass of labelHandling" << LOGDEL
            << DTLOGEVAL(vecHP->at(ii)->virtualClassName())
          );
        }        
        choicesFun[ii] = dynamic_cast< labelHandling const * >(vecHP->at(ii))->getLabel().c_str();
      }
      choiceParamP->disable();
      choiceParamP->setValue(vecHP->size(), choicesFun, 0);
      choiceParamP->enable();

      if ( (choiceParamP->getNumChoices() > 0) && (label<choiceParamP->getNumChoices()) ) {
        if ( !(choiceParamP->getLabel(label) == labelStr) ) {
          label = 0;
        }
        choiceParamP->setValue(label);
      }
    }
  private:
    abstractModule();
  };
}
#endif	/* ABSTRACTMODULE_H */

