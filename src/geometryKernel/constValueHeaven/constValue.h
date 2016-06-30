#ifndef CONSTVALUE_H
#define	CONSTVALUE_H

#include <string>
#include <logMe/dtMacros.h>
#include <xmlHeaven/qtXmlPrimitive.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class constValue : public labelHandling {
  public:
    dt__classOnlyName(constValue);    
    constValue( constValue const & orig );
    virtual ~constValue();
    static constValue * create(
      std::string const & type, 
      std::string const & label, 
      std::string const & valueStr
    );
    virtual constValue * clone( void ) const = 0;    
    virtual void dump(void) const = 0;
    virtual float getValue(void) const;
    virtual void setValue(float const toSet);
    virtual void setRange(float const min, float const max);
    virtual void writeToElement(QDomDocument & doc, ::QDomElement & element) = 0;
    virtual float getMin(void) const;
    virtual float getMax(void) const;
    virtual void resolveConstraint( 
      vectorHandling< constValue * > const * const cVArr
    );
  protected:
    constValue();    
  private:
    float _value;
  };
}
#endif	/* CONSTVALUE_H */

