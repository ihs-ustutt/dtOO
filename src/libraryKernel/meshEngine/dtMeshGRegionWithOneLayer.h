#ifndef dtMeshGRegionWithOneLayer_H
#define	dtMeshGRegionWithOneLayer_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh3DOperator.h"

class GEntity;
class MVertex;
class MElement;

namespace dtOO {
  class dtGmshFace;
  class dtGmshModel;
  class dtGmshRegion;
  
  class dtMeshGRegionWithOneLayer : public dtMesh3DOperator {
    public:
      dt__classOnlyName(dtMeshGRegionWithOneLayer);          
      dtMeshGRegionWithOneLayer();
      virtual ~dtMeshGRegionWithOneLayer();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO
      );            
      virtual void operator()( dtGmshRegion * dtgr );
    private:
      std::string _faceMaster;
      std::string _faceSlave;
  };
}

#endif	/* dtMeshGRegionWithOneLayer_H */

