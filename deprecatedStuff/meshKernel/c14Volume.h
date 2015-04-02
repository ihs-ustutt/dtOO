#ifndef C14VOLUME_H
#define	C14VOLUME_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/labeledPrimType.h>
#include <interfaceHeaven/ptrHandling.h>
#include "dtMacroMesh.h"
#include "cardinalDirTransfiniteVolume.h"

namespace covise {
  class coDoSet;
}

namespace dtOO {
  class analyticGeometry;
  class analyticSurface;
  class analyticFunction;
  class constValue;  
  class blockGridModel;
  
  class c14Volume : public cardinalDirTransfiniteVolume,
                    public dtMacroMesh {
    public:
      dt__classOnlyName(c14Volume); 
      c14Volume();
      virtual ~c14Volume();
      virtual void init(
        QDomElement const & element,
        vectorHandling< constValue * > const * const cValP,
        vectorHandling< analyticFunction * > const * const sFunP,
        vectorHandling< analyticGeometry * > const * const depAGeoP,
        vectorHandling< boundedVolume * > const * const depBVolP
      );    
      virtual covise::coDoSet * toCoDoSet(char const * const str) const;
      virtual void makeGrid( void );
      virtual void makePreGrid( void );
      virtual covise::coDoSet * toCoDoUnstructuredGrid(char const * const str) const;  
      virtual covise::coDoSet * toCoDoSurfUnsGrid(char const * const str, int const boundFaceNum) const; 
      blockGridModel * refToBlockGridModel( void );
    private:
    private:
      vectorHandling< constValue * > _valCValTwins;   
      ptrHandling< blockGridModel > _gModel;
  };
}
#endif	/* C14VOLUME_H */

