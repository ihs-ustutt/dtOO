#ifndef combineGmsh_H
#define	combineGmsh_H

#include <logMe/dtMacros.h>
#include "gmshBoundedVolume.h"

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class dtGmshModel;
  class map3dTo3d;
   
  class combineGmsh : public gmshBoundedVolume {
    public:
      dt__class(combineGmsh, boundedVolume);
      combineGmsh();
      virtual ~combineGmsh();
      virtual void init( 
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV
      );    
//	  virtual void makePreGrid( void );
      virtual void makeGrid( void );      
    private:
      bVPtrVec _bV;
      vectorHandling< dtGmshModel const * > _dtGM;
      float _duplicatePrecision;
  };
}
#endif	/* combineGmsh_H */

