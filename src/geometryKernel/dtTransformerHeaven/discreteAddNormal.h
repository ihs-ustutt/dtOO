#ifndef discreteAddNormal_H
#define	discreteAddNormal_H

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
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual vectorHandling< analyticFunction * > 
    apply( vectorHandling< analyticFunction * > const * const aFP ) const;
  private:
    dt__pH(vec3dTwoD) _tt;
//    dtVector3 _min;
//    dtVector3 _max;
//    int _nSmooth;
    dtVector3 _nf;
    int _nU;
    int _nV;
    int _order;
    int _nSteps;
  };
}    

#endif	/* discreteAddNormal_H */
