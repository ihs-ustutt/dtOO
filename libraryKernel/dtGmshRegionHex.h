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
    dt__CLASSSTD(dtGmshRegionHex, GEntity);
    dtGmshRegionHex( 
      GModel *m, 
      int tag, 
      const std::vector<int> & geTag,
		  const std::list< dtGmshFace * > &faces, 
      const std::vector<int> & ori
    );
    void meshTransfinite( int const nU, int const nV, int const nW );
    void setGrading( std::vector< float > const & grading, std::vector< float > & type );
  private:
    std::vector< int > _hexEdges;
  };
}
#endif	/* DTGMSHREGIONHEX_H */

