#ifndef WRITEIGES_H
#define	WRITEIGES_H

#include <abstractModule.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/coDoSetHandling.h>

namespace dtOO {
  class analyticGeometry;
  
  class writeIges : public covise::coModule,
                    public abstractModule {
  public:
    DTCLASSLOGMETHOD(writeIges);
    DTCLASSNAMEMETHOD(writeIges);
    writeIges(int argc, char *argv[]);
    virtual ~writeIges();
  private:
    virtual int compute(const char *);
  private:
    covise::coInputPort * _pIn_anGeometryP;
    covise::coFileBrowserParam* _p_xmlFilebrowser;
    covise::coFileBrowserParam* _p_igesFilebrowser;
          
    coDoSetHandling< analyticGeometry * > _aGeo;
  };
}
#endif	/* WRITEIGES_H */

