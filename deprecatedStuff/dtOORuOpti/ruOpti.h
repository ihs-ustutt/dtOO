#ifndef RUOPTI_H
#define	RUOPTI_H

#include <api/coModule.h>
#include <abstractModule.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/coDoSetHandling.h>

namespace dtOO {
  class constValue;
  class dtXmlParser;
  class scaFunction;
  
  class ruOpti : public covise::coModule,
                 public abstractModule {
  public:
    dt__classOnlyName(ruOpti);    
    ruOpti(int argc, char *argv[]);
    virtual ~ruOpti();
    virtual int compute(const char *); 
    void param(const char* paramName, bool inMapLoading);
  private:
      covise::coInputPort * _pIn_constValueP;
      covise::coInputPort * _pIn_resValueP;
      covise::coFileBrowserParam * _p_xmlFilebrowser;      
      
      dtXmlParser * _parserP;
      coDoSetHandling< constValue * > _cVal;
      vectorHandling< scaFunction * > _sFun;
      
      int _exCounter;
  };
}

#endif	/* RUOPTI_H */

