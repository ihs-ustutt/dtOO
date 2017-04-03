#ifndef ABSTRACTMODULE_H
#define	ABSTRACTMODULE_H


#include <string.h>
#include <api/coChoiceParam.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>

namespace dtOO {  
  class abstractModule {    
  public:
    dt__classOnlyName(abstractModule);
    virtual ~abstractModule();
    static std::string initializeLogFile( std::string const logFileName ) ; 
    static void closeLogFile( void );
    static void updateChoiceParam(
      covise::coChoiceParam * choiceParamP, 
      std::vector< std::string > const * const vecHP
    );    
    static void updateChoiceParam(
      covise::coChoiceParam * choiceParamP, 
      std::vector< std::string > const & vecHP
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

      std::vector<std::string> choicesFun(vecHP->size(), "empty");
      for (int ii = 0;ii<vecHP->size();ii++) {
        if ( ! dynamic_cast< labelHandling const * >( vecHP->at(ii) ) ) {
          dt__throw(
            updateChoiceParam(), 
            << "vecHP< T >: T is not a baseclass of labelHandling" << std::endl
            << dt__eval(vecHP->at(ii)->virtualClassName())
          );
        }        
        choicesFun[ii] = dynamic_cast< labelHandling const * >(vecHP->at(ii))->getLabel();
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
    static std::string blankReConvert(std::string const & str);
  private:
    abstractModule();
  };
}
#endif	/* ABSTRACTMODULE_H */

