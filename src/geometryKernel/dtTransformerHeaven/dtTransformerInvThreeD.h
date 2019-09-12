#ifndef dtTransformerInvThreeD_H
#define	dtTransformerInvThreeD_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtTransformer.h"

class QDomElement;

namespace dtOO {  
  class dtTransformerInvThreeD : public dtTransformer {
  public:
    dt__classOnlyName(dtTransformerInvThreeD);
    dtTransformerInvThreeD();
    virtual ~dtTransformerInvThreeD();
    dtTransformerInvThreeD(dtTransformerInvThreeD const & orig);
    //
    // overload
    //
    virtual dtTransformerInvThreeD * clone( void ) const = 0;
    virtual dtTransformerInvThreeD * create( void ) const = 0;    
    virtual std::vector< dtPoint3 > apply(
      std::vector< dtPoint3 > const * const toTrans 
    ) const = 0;
    virtual std::vector< dtVector3 > apply(
      std::vector< dtVector3 > const * const toTrans 
    ) const = 0;
    virtual std::vector< dtPoint3 > retract(
      std::vector< dtPoint3 > const * const toRetract
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
    std::vector< dtPoint3 > apply(
      std::vector< dtPoint3 > const & toTrans 
    ) const;
    std::vector< dtPoint3 > retract(
      std::vector< dtPoint3 > const & toRetract
    ) const;
    dtPoint3 apply(dtPoint3 const & pp) const;
    dtPoint3 retract(dtPoint3 const & toRetract) const;    
  };
}

#endif	/* dtTransformerInvThreeD_H */

