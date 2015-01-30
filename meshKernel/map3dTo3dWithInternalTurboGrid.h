#ifndef MAP3DTO3DWITHINTERNALTURBOGRID_H
#define	MAP3DTO3DWITHINTERNALTURBOGRID_H

#include <logMe/dtMacros.h>
#include "boundedVolume.h"
#include <interfaceHeaven/ptrHandling.h>

//namespace moab {
//  class Interface;
//}

class MVertex;
class MElement;
class GFace;
class GRegion;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class map2dTo3d;
  class map3dTo3d;
//  class dtGmshModel;
  
  class map3dTo3dWithInternalTurboGrid : public boundedVolume {
  public:
    map3dTo3dWithInternalTurboGrid();
    virtual ~map3dTo3dWithInternalTurboGrid();
    virtual void init( 
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV
    );    
    virtual void makeGrid(void);
    virtual void makePreGrid(void);
  	virtual vectorHandling< renderInterface * > getRender( void ) const;
  	virtual vectorHandling< renderInterface * > getExtRender( void ) const;    
	  virtual std::vector< std::string > getMeshTags( void ) const;
  private:
//    ptrHandling< moab::Interface > _mb;
//    ptrHandling< dtGmshModel > _gm;
    std::vector< MVertex * > _vertices;
    std::vector< MElement * > _elements;
    std::vector< GFace * > _faces;
    std::vector< GRegion * > _regions;   
    std::vector< std::string > _faceLabels;
    std::vector< std::string > _regionLabels;
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

