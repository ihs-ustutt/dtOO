#ifndef dtGmshMeshBoundaryLayer_H
#define	dtGmshMeshBoundaryLayer_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtGmshFace;
  class dtGmshRegion;
  
  class dtGmshMeshBoundaryLayer {
    public:
      dt__CLASSNAME(dtGmshMeshBoundaryLayer);      
      dtGmshMeshBoundaryLayer();
      dtGmshMeshBoundaryLayer( 
        float const & thickness, std::vector< float > const & spacing,
        float const & maxDihedralAngle,
        int const nSmoothingSteps, int const nShrinkingSteps 
      );
      virtual ~dtGmshMeshBoundaryLayer();
      void operator()( 
        dtGmshRegion * region,
        std::list< dtGmshFace const * > const & face, 
        std::vector< int > const & ori,
        std::list< dtGmshFace const * > const & fface, 
        std::vector< int > const & fori 		      
      );      
    private:
      float _thickness;
      int _nSmoothingSteps;
      int _nShrinkingSteps;
      float _maxDihedralAngle;
      std::vector< float > _spacing;
  };
}

#endif	/* dtGmshMeshBoundaryLayer_H */

