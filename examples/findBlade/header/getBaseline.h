#ifndef GETBASELINE_H
#define	GETBASELINE_H

#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class scaFunction;
  class getBaseline : public dtTransformer {
  public:    
    dt__class(getBaseline, dtTransformer);  
    getBaseline();
    virtual ~getBaseline();
    virtual bool isNecessary( void ) const;
    virtual std::vector< dtPoint2 * > apply( std::vector< dtPoint2 * > const * const toTrans ) const;  
  };
}    

#endif	/* GETBASELINE_H */