#ifndef CONSTVALUEGENERATOR_H
#define	CONSTVALUEGENERATOR_H

#include <api/coModule.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/coDoSetHandling.h>
#include <abstractModule.h>

namespace covise {
  class coFileBrowserParam;
  class coBooleanParam;
  class coChoiceParam;
  class coFloatSliderParam;
  class coIntScalarParam;
}
namespace dtOO {
  class constValue;
  class dtXmlParser;
  
  class constValueGenerator : public covise::coModule,
                              public abstractModule {
    public:
      dt__CLASSNAME(constValueGenerator);    
      constValueGenerator(int argc, char *argv[]);
    private:
      virtual int compute(char const * str);    
      void param(const char* paramName, bool inMapLoading);
      void parseXmlFile(void);
      void saveState(void);
      void loadState( void );
      void loadStateToConst(void);      
    private:
      /* ---------------------------------------------------------------------- */
      /* parsing */
      /* ---------------------------------------------------------------------- */
      covise::coFileBrowserParam * _p_xmlFilebrowser;
      covise::coBooleanParam * _p_parseXml;
      /* ---------------------------------------------------------------------- */
      /* constValues */
      /* ---------------------------------------------------------------------- */
      covise::coChoiceParam * _p_constValueChoiceParam;    
      covise::coFloatSliderParam * _p_constValueFloatSliderParam;
      covise::coIntScalarParam * _p_constValueIntParam;
      covise::coChoiceParam * _p_constValueSetChoiceParam;    
      covise::coFloatSliderParam * _p_constValueSetFloatSliderParam;      
      /* ---------------------------------------------------------------------- */
      /* loading and saving */
      /* ---------------------------------------------------------------------- */    
      covise::coFileBrowserParam * _p_xmlStateFilebrowser;
      covise::coStringParam * _p_stateName;
      covise::coBooleanParam* _p_saveState;
      covise::coBooleanParam* _p_loadState;         
      covise::coChoiceParam* _p_stateChoiceParam;

      covise::coInputPort * _pIn_constValueP;
      covise::coOutputPort * _pOut_constValueP;
      
      dtXmlParser * _dtXmlParserP;
      coDoSetHandling< constValue * > _cVal;
      vectorHandling< vectorHandling< constValue * > > _cValSet;
    };
}
#endif	/* CONSTVALUEGENERATOR_H */

