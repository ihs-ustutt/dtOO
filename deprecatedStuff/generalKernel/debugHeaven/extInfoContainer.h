#ifndef EXTINFOCONTAINER_H
#define	EXTINFOCONTAINER_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/coDoSetHandling.h>

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class analyticFunction;
  class analyticGeometry;
  
  class extInfoContainer : public coDoSetInterface {
  public:
    dt__classOnlyName(extInfoContainer);
    extInfoContainer();
    extInfoContainer(const extInfoContainer& orig);
    virtual ~extInfoContainer();
    void clear( void );
    void addNoClone( analyticFunction * sFunP );
    void addNoClone( analyticGeometry * aGeoP );
    void add( analyticGeometry const * const aGeoP );
    void add( extInfoContainer const * const eIC );
    bool isEmpty( void ) const;
    coDoSetHandling< analyticFunction * > & getRefToScaFunction( void );
    coDoSetHandling< analyticGeometry * > & getRefToAnGeo( void );
    int getNumScaFunctions( void ) const;
    int getNumAnGeos( void ) const;
  private:
    coDoSetHandling< analyticFunction * > _sFun;
    coDoSetHandling< analyticGeometry * > _aGeo;
  };
}
#endif	/* EXTINFOCONTAINER_H */

