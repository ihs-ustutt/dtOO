#ifndef CONSTVALUE_H
#define	CONSTVALUE_H

#include <string>
#include <vector>
#include <logMe/dtMacros.h>
#include <xmlHeaven/qtXmlPrimitive.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/renderInterface.h>

namespace dtOO {
  class constValue : public labelHandling {
  public:
    dt__classOnlyName(constValue);    
    constValue();
    constValue( constValue const & orig );
    virtual ~constValue();
    virtual constValue * clone( void ) const = 0;    
    virtual void dump(void) const = 0;
    float getValue(void) const;
    void setValue(float const toSet);
    virtual void setRange(float const min, float const max);
    virtual void writeToElement(QDomDocument & doc, QDomElement & element) = 0;
    virtual float getMin(void) const;
    virtual float getMax(void) const;
  private:
    float _value;
  };
}
#endif	/* CONSTVALUE_H */

