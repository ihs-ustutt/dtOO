#ifndef DTPOINT2_SURFACEEQUIDISTANTPOINT_H
#define	DTPOINT2_SURFACEEQUIDISTANTPOINT_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class dtSurface;
  
  class dtPoint2_surfaceEquidistantPoint {
  public:
    dt__classOnlyName(dtPoint2_surfaceEquidistantPoint);    
    dtPoint2_surfaceEquidistantPoint();
    dtPoint2_surfaceEquidistantPoint( dtSurface const * const dtS, int const & nU, int const nV );
    virtual ~dtPoint2_surfaceEquidistantPoint();
    twoDArrayHandling< dtPoint2 > result( void );
  private:
    void writeTableToLog( void ) const;
  private:
    twoDArrayHandling< dtPoint2 > _pUV;
    twoDArrayHandling< dtPoint3 > _pXYZ;
    twoDArrayHandling< dtVector2 > _dMS;
    std::vector< float > _sumM;
    std::vector< float > _sumS;
  };
}
#endif	/* DTPOINT2_SURFACEEQUIDISTANTPOINT_H */

