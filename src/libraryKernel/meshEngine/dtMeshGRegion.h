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
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
      labeledVectorHandling< dtMeshOperator * > const * const mO
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