#ifndef bVOPostBLGmsh_H
#define	bVOPostBLGmsh_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class gmshBoundedVolume;
  class dtGmshRegion;
  
  class bVOPostBLGmsh : public bVOInterface {
  public:
    dt__classOnlyName(bVOPostBLGmsh);
    bVOPostBLGmsh();
    virtual ~bVOPostBLGmsh();
    virtual void init(
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void postUpdate( void );
  private:
    float _thickness;
    std::vector< float > _spacing;
    int _nSmoothingSteps;
    int _nShrinkingSteps;
    float _maxDihedralAngle;
    std::vector< std::string > _faceLabel;
    std::vector< int > _faceOrientation;
    std::vector< std::string > _fixedFaceLabel;
    std::vector< int > _fixedFaceOrientation;
    dtGmshRegion * _dtR;
  };
}
#endif	/* bVOPostBLGmsh_H */

