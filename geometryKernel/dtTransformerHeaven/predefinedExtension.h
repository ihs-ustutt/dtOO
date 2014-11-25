#ifndef PREDEFINEDEXTENSION_H
#define	PREDEFINEDEXTENSION_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;

  class predefinedExtension : public dtTransformer {
  public:    
    dt__CLASSNAME(predefinedExtension);
    predefinedExtension();
    predefinedExtension( predefinedExtension const & orig );
    virtual ~predefinedExtension();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;       
    virtual bool isNecessary( void ) const;
    virtual void init( 
      QDomElement const * transformerElementP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP,
      vectorHandling< analyticGeometry * > const * const depAGeoP 
    );
    virtual void init( 
      QDomElement const * transformerElementP, 
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP 
    );    
    virtual vectorHandling< analyticFunction * > apply( vectorHandling< analyticFunction * > const * const sFunP ) const;  
  private:
    float _yIn;
    float _yOut;
  };
}    

#endif	/* PREDEFINEDEXTENSION_H */

