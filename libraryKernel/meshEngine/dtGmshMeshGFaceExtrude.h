#ifndef DTGMSHMESHGFACEEXTRUDE_H
#define	DTGMSHMESHGFACEEXTRUDE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtGmshFace;
  class dtGmshRegion;
  
  class dtGmshMeshGFaceExtrude {
    public:
      dt__CLASSNAME(dtGmshMeshGFaceExtrude);      
      dtGmshMeshGFaceExtrude();
      dtGmshMeshGFaceExtrude( 
        float const & thickness, float const & maxDihedralAngle,
        int const nSmoothingSteps 
      );
      virtual ~dtGmshMeshGFaceExtrude();
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
      float _maxDihedralAngle;
  };
}

#endif	/* DTGMSHMESHGFACEEXTRUDE_H */

