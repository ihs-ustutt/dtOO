#ifndef geomShape_readIgesOCC_H
#define	geomShape_readIgesOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class geomShape_readIgesOCC {
  public:
    dt__classOnlyName(geomShape_readIgesOCC);    
    geomShape_readIgesOCC();
    geomShape_readIgesOCC( std::string const & filename );
    virtual ~geomShape_readIgesOCC();
    vectorHandling< dtCurve * > resultCurveVectorHandling( void ) const;
    vectorHandling< dtSurface * > resultSurfaceVectorHandling( void ) const;
    ptrVectorHandling< dtCurve > const & resultCurveRef( void ) const;
    ptrVectorHandling< dtSurface > const & resultSurfaceRef( void ) const;    
  private:
    ptrVectorHandling< dtCurve > _dtC;
    ptrVectorHandling< dtSurface > _dtS;
  };
}
#endif	/* geomShape_readIgesOCC_H */

