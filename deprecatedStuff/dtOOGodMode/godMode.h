#ifndef godMode_H
#define	godMode_H

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
  
  class godMode : public covise::coModule,
                  public abstractModule {
  public:
    dt__CLASSNAME(godMode);      
    godMode(int argc, char *argv[]);    
  private:
    virtual int compute(char const * str);    
    virtual ~godMode();
  private:
      covise::coInputPort * _pIn_extInfoContainerP;      
      covise::coOutputPort * _pOut_sFunP;
      covise::coOutputPort * _pOut_aGeoP;
  };
}
#endif	/* godMode_H */

