#ifndef CVOPTIMIZER_H
#define	CVOPTIMIZER_H

#include <api/coModule.h>
#include <abstractModule.h>
#include <logMe/dtMacros.h>
#include <optimizableInterface.h>
#include <interfaceHeaven/coDoSetHandling.h>
#include <api/coCallFeedback.h>

namespace dtOO {  
  class constValue;
  class eoLib;
  
  class cvOptimizer : public covise::coModule,
                      public abstractModule,
                      public optimizableInterface {
  public:
    dt__classOnlyName(cvOptimizer);
    cvOptimizer(int argc, char *argv[]);
    virtual ~cvOptimizer();
    virtual int compute(const char *); 
    void param(const char* paramName, bool inMapLoading);
    virtual vectorHandling< constValue * > & getRefToParameters( void );
    virtual float characterizeMe( void ) const;
    virtual int getNParameters( void ) const; 
  private:
    coDoSetHandling< constValue * > _cVal;
    coDoSetHandling< constValue * > _resValue;
    covise::coInputPort * _pIn_constValueP;
    covise::coInputPort * _pIn_optiControlP;
    covise::coFileBrowserParam * _p_xmlFilebrowser;    
    covise::coChoiceParam * _resValueChoiceParam;     
    covise::coBooleanParam * _p_startCycle;
    bool _initialized;
    eoLib * _eoLib;
    coCallFeedback * _cVcCF;
    resValueMap _fM;
  };
}

#endif	/* CVOPTIMIZER_H */

