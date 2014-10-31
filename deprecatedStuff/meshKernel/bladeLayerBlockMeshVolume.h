#ifndef BLADELAYERBLOCKMESHVOLUME_H
#define	BLADELAYERBLOCKMESHVOLUME_H

#include "bladeLayerVolume.h"

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class bladeLayerBlockMeshVolume : public bladeLayerVolume {
  public:
    bladeLayerBlockMeshVolume();
    bladeLayerBlockMeshVolume(const bladeLayerBlockMeshVolume& orig);
    virtual ~bladeLayerBlockMeshVolume();
    void init(
      QDomElement const & element,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< scaFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );
    void writeGrid( void );
    void writeCheapGrid( void );
    virtual covise::coDoSet * toCoDoUnstructuredGrid(char const * const str) const;
  private:
    void writeDict( std::string const casePath ) const;    
  private:
    std::string _caseDirectory;

  };
}
#endif	/* BLADELAYERBLOCKMESHVOLUME_H */

