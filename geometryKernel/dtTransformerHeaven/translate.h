#ifndef TRANSLATE_H
#define	TRANSLATE_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class translate : public dtTransformer {
  public:    
    dt__CLASSNAME(translate);
    translate();
    translate( translate const & orig );
    virtual ~translate();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const;
    virtual void init( 
      QDomElement const * transformerElementP, 
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );
    virtual void init( 
      QDomElement const * transformerElementP, 
      baseContainer const * const bC, 
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );    
    virtual vectorHandling< analyticFunction * > apply( vectorHandling< analyticFunction * > const * const sFunP ) const; 
    virtual void handleDtVector3(std::string const name, dtVector3 const value);
    virtual void handleDtVector2(std::string const name, dtVector2 const value);    
  private:
    dtVector2 _v2;
    dtVector3 _v3;
  };    
}

#endif	/* TRANSLATE_H */

