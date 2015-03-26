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
        float const & thickness, int const nSmoothingSteps 
      );
      virtual ~dtGmshMeshGFaceExtrude();
      void operator()( 
        dtGmshRegion const * const region,
        std::list< dtGmshFace const * > const & face, 
        std::vector< int > const & ori);      
    private:
      float _thickness;
      int _nSmoothingSteps;
  };
}

#endif	/* DTGMSHMESHGFACEEXTRUDE_H */

