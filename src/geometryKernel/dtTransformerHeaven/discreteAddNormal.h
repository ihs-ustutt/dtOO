#ifndef discreteAddNormal_H
#define	discreteAddNormal_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec3dTwoD;
  
  class discreteAddNormal : public dtTransformer {
    public:    
      dt__classOnlyName(discreteAddNormal);
      discreteAddNormal();
      discreteAddNormal(discreteAddNormal const & orig);
      virtual ~discreteAddNormal();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      virtual void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG 
      );
      virtual aFPtrVec 
      apply( aFPtrVec const * const aFP ) const;
    private:
      dt__pH(vec3dTwoD) _tt;
      dtVector3 _nf;
      dtInt _nU;
      dtInt _nV;
      dtInt _order;
      dtInt _nSteps;
      dtInt _skinOrderMin;
      dtInt _skinOrderMax;
      dtInt _skinNIterations;
      bool _closeU;
      dtReal _closeSmooth;
      static bool _registrated;    
  };
}    

#endif	/* discreteAddNormal_H */

