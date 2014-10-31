#ifndef BLADELAYERVOLUME_H
#define	BLADELAYERVOLUME_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/labeledPrimType.h>
#include "simpleBlockStructure.h"
#include "cardinalDirVolume.h"

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class constValue;
  
  class bladeLayerVolume : public cardinalDirVolume,
                           public simpleBlockStructure
  {
  public:
    DTCLASSNAMEMETHOD(bladeLayerVolume);
    DTCLASSLOGMETHOD(bladeLayerVolume);    
    bladeLayerVolume();
    virtual ~bladeLayerVolume();
    virtual void init(
                   QDomElement const & element,
                   vectorHandling< constValue * > const * const cValP,
                   vectorHandling< scaFunction * > const * const sFunP,
                   vectorHandling< analyticGeometry * > const * const depAGeoP,
                   vectorHandling< boundedVolume * > const * const depBVolP
                 );    
//    virtual void writeGrid( void );
//    virtual void writeCheapGrid( void );
    virtual covise::coDoSet * toCoDoSet(char const * const str) const;
//    virtual covise::coDoSet * toCoDoUnstructuredGrid(char const * const str) const;   
    void pushGridToSimpleBlockStructure( void );
    std::vector< std::vector< int > > getCouplingVertex( void ) const;
  private:  
    vectorHandling< constValue * > _valCValTwins;
  };
}

#endif	/* BLADELAYERVOLUME_H */

