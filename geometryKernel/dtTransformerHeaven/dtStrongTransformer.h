#ifndef dtStrongTransformer_H
#define	dtStrongTransformer_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtTransformer.h"

class QDomElement;

namespace dtOO {  
  class dtStrongTransformer : public dtTransformer {
  public:
    dt__classOnlyName(dtStrongTransformer);
    dtStrongTransformer();
    virtual ~dtStrongTransformer();
    dtStrongTransformer(dtStrongTransformer const & orig);
    //
    // overload
    //
    virtual dtStrongTransformer * clone( void ) const = 0;
    virtual dtStrongTransformer * create( void ) const = 0;    
    virtual std::vector< dtVector3 > apply(
      std::vector< dtVector3 > const * const toTrans 
    ) const = 0;
    
    virtual std::vector< dtVector3 > retract(
      std::vector< dtVector3 > const * const toRetract
    ) const = 0;
    //
    //
    //
    using dtTransformer::apply;
    using dtTransformer::retract;
    std::vector< dtVector3 > apply(
      std::vector< dtVector3 > const & toTrans 
    ) const;
    std::vector< dtVector3 > retract(
      std::vector< dtVector3 > const & toRetract
    ) const;
    dtVector3 apply(dtVector3 const & pp) const;
    dtVector3 retract(dtVector3 const & toRetract) const;
  };
}

#endif	/* dtStrongTransformer_H */

