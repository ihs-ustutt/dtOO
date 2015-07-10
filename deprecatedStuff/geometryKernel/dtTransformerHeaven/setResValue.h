#ifndef SETRESVALUE_H
#define	SETRESVALUE_H

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

class scaFunction;

namespace dtOO {
  class setResValue : public dtTransformer {
  public:    
    dt__classOnlyName(setResValue);
    setResValue();
    virtual ~setResValue();
    virtual bool isNecessary( void ) const;
    virtual void init( ::QDomElement * transformerElementP,
                       vectorHandling< constValue * > const * const cValP,
                       vectorHandling< scaFunction * > const * const sFunP);
    virtual vectorHandling< scaFunction * > apply( vectorHandling< scaFunction * > const * const sFunP ) const;  
    virtual void handleFloat(std::string const name, float const value);
//    virtual void handleScaFunction(std::string const name, scaFunction const * value);
//    virtual void handleBool(std::string const name, bool const value);
  private:
    float _resValue;
  };
}    

#endif	/* SETRESVALUE_H */

