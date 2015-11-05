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
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtMeshOperator * > const * const mO
    );    
    void operator()( dtGmshRegion * dtgr );    
  private:
    void createPyramids( dtGmshRegion * dtgr );
    void doLaplacianSmooth( 
      dtOVMMesh & ovm, float const & minQShapeMetric 
    ) const;
  private:
    float _relax;
    float _minQShapeMetric;
    int _nPyramidOpenSteps;
    int _nLaplacianSmooths;
  };
}
#endif	/* DTMESHGREGION_H */