#ifndef WRITEXY_H
#define	WRITEXY_H

#include <api/coModule.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/coDoSetHandling.h>
#include <abstractModule.h>

namespace dtOO {
  class analtyticGeometry;
  
  class aGeoToXYZ : public covise::coModule,
                    public abstractModule {
  public:
    dt__CLASSNAME(aGeoToXYZ);    
    aGeoToXYZ(int argc, char *argv[]); 
    virtual ~aGeoToXYZ();
    virtual int compute(char const * str);        
  private:
    covise::coFileBrowserParam* _p_xmlFilebrowser;    
    covise::coFileBrowserParam* _p_xyzFilebrowser;
    covise::coInputPort * _pIn_aGeoP; 

      coDoSetHandling< analyticGeometry * > _aGeo;      
  };
}
#endif	/* WRITEXY_H */

