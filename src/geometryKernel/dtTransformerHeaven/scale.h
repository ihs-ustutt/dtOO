#ifndef scale_H
#define	scale_H

#include "dtStrongTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class scale : public dtStrongTransformer {
  public:    
    dt__classOnlyName(scale);
    scale();
    virtual ~scale();
    scale(const scale& orig);    
    virtual dtStrongTransformer * clone( void ) const;
    virtual dtStrongTransformer * create( void ) const;       
    virtual bool isNecessary( void ) const;
    void init(
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG 
    );
    virtual std::vector< dtPoint3 > apply( 
      std::vector< dtPoint3 > const * const toTrans 
    ) const;
    virtual std::vector< dtPoint3 > retract(
      std::vector< dtPoint3 > const * const toRetract
    ) const;
    virtual std::vector< dtVector3 > apply( 
      std::vector< dtVector3 > const * const toTrans 
    ) const;
    virtual std::vector< dtVector3 > retract(
      std::vector< dtVector3 > const * const toRetract
    ) const;        
  private:
    dtVector3 _scale;
  };
}
#endif	/* scale_H */

