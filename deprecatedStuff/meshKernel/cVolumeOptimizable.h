#ifndef CVOLUMEOPTIMIZABLE_H
#define	CVOLUMEOPTIMIZABLE_H

#include <logMe/dtMacros.h>

#include "cVolume.h"
#include <interfaceHeaven/optimizationHandling.h>
#include <interfaceHeaven/optimizationCyclingDecorator.h>

namespace dtOO {
  class cVolumeOptimizable : public cVolume,
                             public optimizationHandling {
  public:
    DTCLASSNAMEMETHOD(cVolumeOptimizable);
    DTCLASSLOGMETHOD(cVolumeOptimizable);
    cVolumeOptimizable();
    cVolumeOptimizable(const cVolumeOptimizable& orig);
    virtual ~cVolumeOptimizable();
    virtual void init(
                   QDomElement const & element,
                   vectorHandling< constValue * > const * const cValP,
                   vectorHandling< scaFunction * > const * const sFunP,
                   vectorHandling< analyticGeometry * > const * const depAGeoP 
                 );    
    virtual float characterizeMe(void);
    virtual float characterizeFailedDesign(void);
//    virtual void doCycle( void );    
  private:
    void readResultCheckMesh( void );
  private:
    float _meshQuality;
    float _meshWorst;
    float _meshFailed;
    bool _tmpVolume;    

  };
}
#endif	/* CVOLUMEOPTIMIZABLE_H */

