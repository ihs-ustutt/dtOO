#ifndef DTOMMESHMANIFOLD_H
#define	DTOMMESHMANIFOLD_H

#include <logMe/dtMacros.h>
#include "dtOMMesh.h"

namespace dtOO {
  class dtOMMeshManifold : public dtOMMesh {
  public:
    dt__CLASSNAME(dtOMMeshManifold);
    dtOMMeshManifold(dtOMMesh const & om, omVertexH const & vH);
    dtOMMeshManifold( dtOMMeshManifold const & orig);
    virtual ~dtOMMeshManifold();
    float minDihedralAngle( void ) const;
    float maxDihedralAngle( void ) const;
    bool divideable( void ) const;
    bool closed( void ) const;
    virtual void update( void );
    dtOMMeshManifold divide(void);
    std::vector< dtOMMeshManifold > divide( float const & angle );
    dtVector3 normal( void ) const;
    MVertex * centerMVertex( void ) const;
  private:
    dtOMMeshManifold subractManifold(omEdgeH const & from, omEdgeH const & to);
  private:
    std::vector< float > _dihedralAngleV;
		std::vector< bool > _isBoundary;
    omVertexH _centerVertex;
  };
}
#endif	/* DTOMMESHMANIFOLD_H */

