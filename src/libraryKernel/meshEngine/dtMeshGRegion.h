#ifndef DTMESHGREGION_H
#define	DTMESHGREGION_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh3DOperator.h"
#include "dtOVMMesh.h"
#include "dtGmshRegion.h"

class MPyramid;

namespace dtOO {
  class dtGmshRegion;
  class dtGmshFace;
  
  class dtMeshGRegion : public dtMesh3DOperator {
  public:
    dt__class(dtMeshGRegion, dtMeshOperator);     
    dtMeshGRegion();
    dtMeshGRegion(const dtMeshGRegion& orig);
    virtual ~dtMeshGRegion();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      lvH_constValue const * const cV,
      lvH_analyticFunction const * const aF,
      lvH_analyticGeometry const * const aG,
      lvH_boundedVolume const * const bV,
      lvH_dtMeshOperator const * const mO
    );    
    void operator()( dtGmshRegion * dtgr );    
  private:
    void createPyramids( dtGmshRegion * dtgr );
    void createOVM(dtGmshRegion * dtgr, dtOVMMesh & ovm);
    static dtReal pyramidHeight( ::MPyramid * pyr );
  private:
    dtReal _relax;
    dtReal _minQShapeMetric;
    dtReal _maxHeight;
    dtInt _nPyramidOpenSteps;
    dtInt _nSmooths;
  };
}
#endif	/* DTMESHGREGION_H */