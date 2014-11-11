#ifndef DTGMSHTRANSFACE_H
#define	DTGMSHTRANSFACE_H

#include <dtLinearAlgebra.h>
#include "dtGmshFace.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class GModel;
class GEdge;

namespace dtOO {
  class map1dTo3d;
  
  class dtGmshTransFace : public dtGmshFace {
  public:
    dt__CLASSNAME(dtGmshTransFace);  
    dtGmshTransFace(GModel *m, int tag, const std::list<GEdge*> &edges, const std::vector< int > & ori);
    dtGmshTransFace(GModel *m, int tag, const std::list<GEdge*> &edges);
    virtual ~dtGmshTransFace();
    virtual void updateFace(void);
    virtual void makeSuitable( void );
  private:
    void blendSurf( vectorHandling< map1dTo3d const * > & curve );
  private:
  };
}
#endif	/* DTGMSHTRANSFACE_H */

