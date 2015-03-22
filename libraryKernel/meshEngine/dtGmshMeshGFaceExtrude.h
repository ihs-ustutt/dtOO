#ifndef DTGMSHMESHGFACEEXTRUDE_H
#define	DTGMSHMESHGFACEEXTRUDE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtGmshFace;
  
  class dtGmshMeshGFaceExtrude {
    public:
      dt__CLASSNAME(dtGmshMeshGFaceExtrude);      
      dtGmshMeshGFaceExtrude();
      dtGmshMeshGFaceExtrude( float const & thickness );
      virtual ~dtGmshMeshGFaceExtrude();
      void operator()( std::list< dtGmshFace const * > const & face, std::vector< int > const & ori);      
    private:
      float _thickness;
  };
}

#endif	/* DTGMSHMESHGFACEEXTRUDE_H */

