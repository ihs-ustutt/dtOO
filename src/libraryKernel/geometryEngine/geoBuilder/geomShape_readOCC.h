#ifndef geomShape_readOCC_H
#define	geomShape_readOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class geomShape_readOCC {
  public:
    dt__classOnlyName(geomShape_readOCC);    
    geomShape_readOCC();
    geomShape_readOCC( std::string const & filename );
    virtual ~geomShape_readOCC();
    vectorHandling< dtCurve * > resultCurveVectorHandling( void ) const;
    vectorHandling< dtSurface * > resultSurfaceVectorHandling( void ) const;
    ptrVectorHandling< dtCurve > const & resultCurveRef( void ) const;
    ptrVectorHandling< dtSurface > const & resultSurfaceRef( void ) const;    
  private:
    ptrVectorHandling< dtCurve > _dtC;
    ptrVectorHandling< dtSurface > _dtS;
  };
}
#endif	/* geomShape_readOCC_H */

