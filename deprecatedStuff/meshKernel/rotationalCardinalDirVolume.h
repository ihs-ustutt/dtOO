#ifndef ROTATIONALCARDINALDIRVOLUME_H
#define	ROTATIONALCARDINALDIRVOLUME_H

#include "cardinalDirVolume.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/coDoSetHandling.h>

namespace covise {
  class coDoSet;  
}

namespace dtOO {
  class analyticGeometry;
  class rotatingSpline;
  class splineCurve3d;
  class scaFunction;
  class constValue;
  
  class rotationalCardinalDirVolume : public cardinalDirVolume {
  public:
    DTCLASSNAMEMETHOD(cardinalDirVolume);
    DTCLASSLOGMETHOD(cardinalDirVolume);      
    rotationalCardinalDirVolume();
    virtual ~rotationalCardinalDirVolume();
    virtual void init(
      QDomElement const & element,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< scaFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< boundedVolume * > const * const depBVolP
    );    
    void writeGrid( void );
    virtual covise::coDoSet * toCoDoUnstructuredGrid(char const * const str) const;  
    virtual covise::coDoSet * toCoDoSet(char const * const str) const;
//    virtual void packToExtInfoContainer( extInfoContainer * const eIC ) const;
    
  private:
    coDoSetHandling< rotatingSpline * > _rS;
    coDoSetHandling< splineCurve3d * > _s3d;
  };
}
#endif	/* ROTATIONALCARDINALDIRVOLUME_H */

