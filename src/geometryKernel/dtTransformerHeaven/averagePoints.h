#ifndef AVERAGEPOINTS_H
#define	AVERAGEPOINTS_H

#include <dtTransformerHeaven/dtTransformer.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class averagePoints : public dtTransformer {
  public:    
    dt__class(averagePoints, dtTransformer);  
    averagePoints();
    averagePoints( averagePoints const & orig );
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;      
    void init( 
      ::QDomElement const * tE, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );    
    virtual ~averagePoints();
    virtual bool isNecessary( void ) const;
    virtual std::vector< dtPoint2 * > apply( std::vector< dtPoint2 * > const * const toTrans ) const;  
  };
}    

#endif	/* AVERAGEPOINTS_H */