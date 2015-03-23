#ifndef DTOMMESHMANIFOLD_H
#define	DTOMMESHMANIFOLD_H

#include <logMe/dtMacros.h>
#include "dtOMMesh.h"

namespace dtOO {
  class dtOMMeshManifold : public dtOMMesh {
  public:
    dt__CLASSNAME(dtOMMeshManifold);
    dtOMMeshManifold(dtOMMesh const & om, omVertexH const & vH);
    virtual ~dtOMMeshManifold();
    float minDihedralAngle( void ) const;
    float maxDihedralAngle( void ) const;
    bool divideable( void ) const;    
    bool closed( void ) const;
    void update( void );
  private:
//    bool _isClosed;
//    bool _isDivideable;
    std::vector< float > _dihedralAngleV;
		std::vector< bool > _isBoundary;
    omVertexH _centerVertex;
  };
}
#endif	/* DTOMMESHMANIFOLD_H */

