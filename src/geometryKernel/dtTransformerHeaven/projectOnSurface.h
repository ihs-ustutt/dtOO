#ifndef PROJECTONSURFACE_H
#define	PROJECTONSURFACE_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticSurface;
  
  class projectOnSurface : public dtTransformer {
    public:    
      dt__classOnlyName(projectOnSurface);
      projectOnSurface();
      virtual ~projectOnSurface();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
      );
      virtual aGPtrVec apply( 
        aGPtrVec const * const toTrans 
      ) const;
    private:
      dt__pH(analyticSurface const) _aS;
      static bool _registrated;    
  };
}
#endif	/* PROJECTONSURFACE_H */

