#ifndef DTGMSHREGIONHEX_H
#define	DTGMSHREGIONHEX_H

#include <dtLinearAlgebra.h>
#include "dtGmshRegion.h"
#include <logMe/dtMacros.h>

class GModel;
class GFace;

namespace dtOO {
  class dtGmshRegionHex : public dtGmshRegion {
  public:
    dt__CLASSNAME(dtGmshRegionHex);    
    dtGmshRegionHex( 
      GModel *m, 
      int tag, 
      const std::vector<int> & geTag,
      const std::list<GFace*> & faces,
      const std::vector<int> & ori
    );
    void reorderBlock( void );
  private:
    std::vector< int > _hexEdges;
  };
}
#endif	/* DTGMSHREGIONHEX_H */

