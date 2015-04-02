#ifndef DTGMSHTRANSPARTAREA_H
#define	DTGMSHTRANSPARTAREA_H

#include <dtLinearAlgebra.h>
#include "dtGmshFace.h"
#include "geometryEngine/dtCurve.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class GModel;
class GEdge;

namespace dtOO {
  class map2dTo3d;
  class dtGmshEdge;
  
  class dtGmshTransPartArea : public dtGmshFace {
    public:
      dt__classOnlyName(dtGmshTransPartArea);
      dtGmshTransPartArea(::GModel *m, int tag, const std::list< ::GEdge * > &edges, const std::vector< int > & ori);
      dtGmshTransPartArea(::GModel *m, int tag, const std::list< ::GEdge * > &edges);
      virtual ~dtGmshTransPartArea();
//      virtual void setMap2dTo3d( map2dTo3d const * const base );
      std::string getMotherSurfaceLabel( void ) const;
      void setMotherSurfaceLabel( std::string const label );
      virtual void updateFace(void);    
    private:
      vectorHandling< dtGmshEdge const * > _edge;    
      std::string _motherSurfaceName;
  };
}

#endif	/* DTGMSHTRANSPARTAREA_H */

