#ifndef ADDCONSTCOORDINATE_H
#define	ADDCONSTCOORDINATE_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class vec2dOneD;
  
  class addConstCoordinate : public dtTransformer{
  public:
    dt__CLASSNAME(addConstCoordinate);   
    addConstCoordinate();
    addConstCoordinate(const addConstCoordinate& orig);
    virtual ~addConstCoordinate();
    virtual dtTransformer * clone( void ) const;
    virtual dtTransformer * create( void ) const;     
    virtual bool isNecessary( void ) const; 
    virtual void init( 
      QDomElement const * transformerElementP,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cValP,
      vectorHandling< analyticFunction * > const * const sFunP
    );
    virtual vectorHandling< analyticFunction * > apply( 
      vectorHandling< analyticFunction * > const * const aFVecP 
    ) const;
    virtual void handleFloat(const std::string name, const float value);
    virtual void handleDtVector3(const std::string name, const dtVector3 value);
  private:
    float _cc;
    dtVector3 _vv;
  };
}

#endif	/* ADDCONSTCOORDINATE_H */

