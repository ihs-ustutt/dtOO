#ifndef WRITEXY_H
#define	WRITEXY_H

#include <api/coModule.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/coDoSetHandling.h>
#include <abstractModule.h>

namespace dtOO {
  class writeXy : public covise::coModule,
                  public abstractModule {
  public:
    dt__classOnlyName(writeXy);    
    writeXy(int argc, char *argv[]); 
    virtual ~writeXy();
    virtual int compute(char const * str);        
  private:
    covise::coFileBrowserParam* _p_xmlFilebrowser;    
    covise::coFileBrowserParam* _p_xyFilebrowser;
    covise::coInputPort * _pIn_sFunP; 

    coDoSetHandling< scaFunction * > _sFun;
  };
}
#endif	/* WRITEXY_H */

