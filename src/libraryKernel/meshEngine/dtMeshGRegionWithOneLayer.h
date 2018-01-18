#ifndef dtMeshGRegionWithOneLayer_H
#define	dtMeshGRegionWithOneLayer_H

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
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        labeledVectorHandling< dtMeshOperator * > const * const mO
      );            
      virtual void operator()( dtGmshRegion * dtgr );
    private:
      std::string _faceMaster;
      std::string _faceSlave;
  };
}

#endif	/* dtMeshGRegionWithOneLayer_H */

