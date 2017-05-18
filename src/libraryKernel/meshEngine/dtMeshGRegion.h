#ifndef DTMESHGREGION_H
#define	DTMESHGREGION_H

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
    static float pyramidHeight( ::MPyramid * pyr );
  private:
    float _relax;
    float _minQShapeMetric;
    float _maxHeight;
    int _nPyramidOpenSteps;
    int _nSmooths;
  };
}
#endif	/* DTMESHGREGION_H */