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
    virtual ~predefinedExtension();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;       
    virtual bool isNecessary( void ) const;
    virtual void init( QDomElement const * transformerElementP, 
                       vectorHandling< constValue * > const * const cValP,
                       vectorHandling< analyticFunction * > const * const sFunP );
    virtual vectorHandling< analyticFunction * > apply( vectorHandling< analyticFunction * > const * const sFunP ) const;  
  private:
    float _alphaIn;
    float _yIn;
    float _alphaOut;
    float _yOut;
  };
}    

#endif	/* PREDEFINEDEXTENSION_H */

