#ifndef vec3dInMap3dTo3dWithMeshedSurface_H
#define	vec3dInMap3dTo3dWithMeshedSurface_H

#include <logMe/dtMacros.h>
#include "boundedVolume.h"
//#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
  class map3dTo3d;
   
  class vec3dInMap3dTo3dWithMeshedSurface : public boundedVolume {
  public:
    dt__CLASSSTD(vec3dInMap3dTo3dWithMeshedSurface, boundedVolume);
    vec3dInMap3dTo3dWithMeshedSurface();
    virtual ~vec3dInMap3dTo3dWithMeshedSurface();
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
    virtual dtGmshModel * getModel( void ) const;
  private:
    dt__pH(map3dTo3d) _m3d;    
    ptrHandling< dtGmshModel > _gm;
    boundedVolume const * _meshedBV;
    std::string _meshedFaceTag;
    dt__pH(map3dTo3d) _recVol;
    dt__pH(analyticGeometry) _recFace;
    
  };
}
#endif	/* vec3dInMap3dTo3dWithMeshedSurface_H */

