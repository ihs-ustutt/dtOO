#ifndef DTMESHGREGION_H
#define	DTMESHGREGION_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh3DOperator.h"
#include "dtOVMMesh.h"

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
  private:
    float _relax;
    float _minQShapeMetric;
    int _nPyramidOpenSteps;
    int _nSmooths;
  };
}
#endif	/* DTMESHGREGION_H */