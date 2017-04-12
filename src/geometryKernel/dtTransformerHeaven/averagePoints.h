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
      baseContainer * const bC,
      cVPtrVec const * const cValP,
      aFPtrVec const * const sFunP,
      aGPtrVec const * const depAGeoP 
    );    
    virtual ~averagePoints();
    virtual bool isNecessary( void ) const;
    virtual std::vector< dtPoint2 * > apply( std::vector< dtPoint2 * > const * const toTrans ) const;  
  };
}    

#endif	/* AVERAGEPOINTS_H */