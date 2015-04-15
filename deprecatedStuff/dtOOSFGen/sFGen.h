#ifndef SFGEN_H
#define	SFGEN_H

#include <api/coModule.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/coDoSetHandling.h>
#include <abstractModule.h>

namespace covise {
  class coFileBrowserParam;
  class coChoiceParam;
}

namespace dtOO {
  class dtXmlParser;
  class scaFunction;
  
  class sFGen : public covise::coModule,
                public abstractModule {
  public:
    dt__classOnlyName(sFGen);   
    sFGen(int argc, char *argv[]);
//  private:
    virtual int compute(char const * str);    
    void param(const char* paramName, bool inMapLoading);
    virtual ~sFGen();
  protected:
    coDoSetHandling< scaFunction * > & getRefToScaFunction( void );
  private:
      covise::coFileBrowserParam * _p_xmlFilebrowser;
      covise::coChoiceParam* _p_functionChoiceParam;
      covise::coChoiceParam* _p_functionSetChoiceParam;
      covise::coBooleanParam * _p_passThrough;
      covise::coInputPort * _pIn_constValueP;
      covise::coInputPort * _pIn_sFunP;      
      covise::coOutputPort * _pOut_sFunP;
      covise::coOutputPort* _pOut_functionP;  
      covise::coOutputPort* _pOut_functionSetP;  
      
      dtXmlParser * _dtXmlParserP;
      coDoSetHandling< constValue * > _cVal;      
      coDoSetHandling< scaFunction * > _sFun;
      vectorHandling< coDoSetHandling< scaFunction * > > _sFunSet;
      
      bool _recreate;
  private:
    void updateFunctionChoiceParam(vectorHandling< scaFunction * > * vecHP);
  };
}
#endif	/* SFGEN_H */

