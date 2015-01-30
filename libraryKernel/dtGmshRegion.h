#ifndef DTGMSHREGION_H
#define	DTGMSHREGION_H

#include <gmsh/GRegion.h>
#include <logMe/dtMacros.h>

class GModel;
class GFace;

namespace dtOO {
  class dtGmshFace;
  
  class dtGmshRegion : public GRegion {
  public:
    dt__CLASSSTD(dtGmshRegion, GEntity);
    dtGmshRegion( GModel *m, int tag );
    dtGmshRegion( GModel *m, int tag, const std::list<GFace*> &faces, const std::vector<int> &ori );
    dtGmshRegion( GModel *m, int tag, const std::list<dtGmshFace*> &faces, const std::vector<int> &ori );
    void meshTransfinite( void );
  private:

  };
}
#endif	/* DTGMSHREGION_H */

