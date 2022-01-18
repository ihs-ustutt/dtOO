#ifndef DTOMMESHMANIFOLD_H
#define	DTOMMESHMANIFOLD_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "dtOMMesh.h"

namespace dtOO {
  class dtOMMeshManifold : public dtOMMesh {
  public:
    dt__classOnlyName(dtOMMeshManifold);
    dtOMMeshManifold(dtOMMesh const & om, omVertexH const & vH);
    dtOMMeshManifold( dtOMMeshManifold const & orig);
    virtual ~dtOMMeshManifold();
    dtReal minDihedralAngle( void ) const;
    dtReal maxDihedralAngle( void ) const;
    bool divideable( void ) const;
    bool closed( void ) const;
    virtual void update( void );
    dtOMMeshManifold divide(void);
    std::vector< dtOMMeshManifold > divide( dtReal const & angle );
    dtVector3 normal( void ) const;
    MVertex * centerMVertex( void ) const;
  private:
    dtOMMeshManifold subractManifold(omEdgeH const & from, omEdgeH const & to);
  private:
    std::vector< dtReal > _dihedralAngleV;
		std::vector< bool > _isBoundary;
    omVertexH _centerVertex;
    dtInt _nTri;
    dtInt _nQuad;
  };
}
#endif	/* DTOMMESHMANIFOLD_H */

