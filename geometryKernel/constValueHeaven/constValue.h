#ifndef CONSTVALUE_H
#define	CONSTVALUE_H

#include <string>
#include <vector>
#include <logMe/dtMacros.h>
#include <xmlHeaven/qtXmlPrimitive.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/renderInterface.h>

namespace dtOO {
  class dtXmlParser;
  
  class constValue : public labelHandling {
  public:
    dt__classOnlyName(constValue);    
    constValue( constValue const & orig );
    virtual ~constValue();
    static constValue * create(
      std::string const & type, 
      std::string const & label, 
      dtXmlParser const * const parser
    );
    virtual constValue * clone( void ) const = 0;    
    virtual void dump(void) const = 0;
    float getValue(void) const;
    void setValue(float const toSet);
    virtual void setRange(float const min, float const max);
    virtual void writeToElement(QDomDocument & doc, ::QDomElement & element) = 0;
    virtual float getMin(void) const;
    virtual float getMax(void) const;
    dtXmlParser const & constRefParser( void ) const;
  protected:
    constValue();    
  private:
    float _value;
    dtXmlParser const * _parser;
  };
}
#endif	/* CONSTVALUE_H */

