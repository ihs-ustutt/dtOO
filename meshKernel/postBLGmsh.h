#ifndef postBLGmsh_H
#define	postBLGmsh_H

#include <logMe/dtMacros.h>
#include "boundedVolume.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class postBLGmsh : public boundedVolume {
  public:
    dt__CLASSSTD(postBLGmsh, boundedVolume);    
    postBLGmsh();
    virtual ~postBLGmsh();
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
  private:
    float _thickness;
    int _intervals;
    float _bias;
    boundedVolume const * _meshedBV;
    std::vector< std::string > _faceLabel;
    std::vector< int > _faceOrientation;
  };
}
#endif	/* postBLGmsh_H */

