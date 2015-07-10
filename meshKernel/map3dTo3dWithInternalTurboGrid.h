#ifndef MAP3DTO3DWITHINTERNALTURBOGRID_H
#define	MAP3DTO3DWITHINTERNALTURBOGRID_H

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"
#include <interfaceHeaven/twoDArrayHandling.h>


class MVertex;
class MElement;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class map2dTo3d;
  class map3dTo3d;
  class dtGmshFace;
  class dtGmshRegion;
  
  class map3dTo3dWithInternalTurboGrid : public gmshBoundedVolume {
  public:
    dt__class(map3dTo3dWithInternalTurboGrid, boundedVolume);    
    map3dTo3dWithInternalTurboGrid();
    virtual ~map3dTo3dWithInternalTurboGrid();
    virtual void init( 
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV
    );    
    virtual void makeGrid(void);
    virtual void makePreGrid(void);
  private:
    std::string _meshFileName;
    dt__pH(map3dTo3d) _channel;
    dt__pH(map2dTo3d) _internal;
    int _nInternalCuts;
    int _nPoints;
    int _nInternals;
    dtVector3 _vv;
    std::string _directory;
    std::string _script;    
  };
}

#endif	/* MAP3DTO3DWITHINTERNALTURBOGRID_H */

