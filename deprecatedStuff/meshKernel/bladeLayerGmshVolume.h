#ifndef BLADELAYERGMSHVOLUME_H
#define	BLADELAYERGMSHVOLUME_H

#include "bladeLayerVolume.h"
//#include "blockGridModel.h"

namespace dtOO {
  class blockGridModel;
  class cardinalDirGModel;
  
  class bladeLayerGmshVolume : public bladeLayerVolume {
  public:
    DTCLASSNAMEMETHOD(bladeLayerGmshVolume);
    DTCLASSLOGMETHOD(bladeLayerGmshVolume);        
    bladeLayerGmshVolume();
    virtual ~bladeLayerGmshVolume();
    void init(
      QDomElement const & element,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< scaFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP,
      vectorHandling< boundedVolume * > const * const depBVolP
    );   
    void writeGrid( void );
    void writeCheapGrid( void );
    virtual covise::coDoSet * toCoDoUnstructuredGrid(char const * const str) const;    
    virtual void packToExtInfoContainer( extInfoContainer * const eIC ) const;
  private:
    blockGridModel * _strucModel;
//    cardinalDirGModel * _unstrucModel;
    extInfoContainer * _eIC;
  };
}
#endif	/* BLADELAYERGMSHVOLUME_H */

