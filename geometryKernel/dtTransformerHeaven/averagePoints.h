#ifndef AVERAGEPOINTS_H
#define	AVERAGEPOINTS_H

#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class averagePoints : public dtTransformer {
  public:    
    dt__CLASSSTD(averagePoints, dtTransformer);  
    averagePoints();
    averagePoints( averagePoints const & orig );
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;      
    virtual ~averagePoints();
    virtual bool isNecessary( void ) const;
    virtual std::vector< dtPoint2 * > apply( std::vector< dtPoint2 * > const * const toTrans ) const;  
  };
}    

#endif	/* AVERAGEPOINTS_H */