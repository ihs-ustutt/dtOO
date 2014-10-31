#ifndef GMSHVOLUME_H
#define	GMSHVOLUME_H

#include <dtLinearAlgebra.h>

#include "cardinalDirVolume.h"
#include "boundedVolume.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace covise {
  class coDoSet;
}
  
namespace dtOO {
  class gmshVolume : public cardinalDirVolume {
  public:
    dt__CLASSNAME(gmshVolume);
    gmshVolume();
    virtual ~gmshVolume();
    virtual void init(
                   QDomElement const & element,
                   vectorHandling< constValue * > const * const cValP,
                   vectorHandling< analyticFunction * > const * const sFunP,
                   vectorHandling< analyticGeometry * > const * const depAGeoP,
                   vectorHandling< boundedVolume * > const * const depBVolP
                 );
    virtual void makeGrid( void );
    virtual covise::coDoSet * toCoDoUnstructuredGrid(char const * const str) const;
    virtual covise::coDoSet * toCoDoSurfUnsGrid(char const * const str, int const boundFaceNum) const;
  };
}
#endif	/* GMSHVOLUME_H */

