#ifndef DTGMSHREGION_H
#define	DTGMSHREGION_H

#include <gmsh/GRegion.h>

class GModel;
class GFace;

namespace dtOO {
  class dtGmshRegion : public GRegion {
  public:
    dtGmshRegion( GModel *m, int tag, const std::list<GFace*> &faces, const std::vector<int> &ori );
    dtGmshRegion( GModel *m, int tag );
    bool containsEdge( GEdge const * const edge ) const;
    bool containsFace( GFace const * const face ) const;  
    void meshTransfinite( void );
  private:

  };
}
#endif	/* DTGMSHREGION_H */

