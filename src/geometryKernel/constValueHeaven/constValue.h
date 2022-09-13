#ifndef constValue_H
#define	constValue_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <xmlHeaven/qtXmlPrimitive.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <mainConceptFwd.h>

namespace dtOO {
  class constValue : public labelHandling {
    public:
      dt__class(constValue, constValue);
      constValue( constValue const & orig );
      constValue( std::string const & label, dtReal const & val );
      virtual ~constValue();
      static constValue * create(
        std::string const & type, 
        std::string const & label, 
        std::string const & valueStr,
        bool const loadable
      );
      virtual constValue * clone( void ) const = 0;    
      virtual void dump(void) const = 0;
      virtual dtReal getValue(void) const;
      virtual dtReal getValuePercent(void) const;
      virtual void setValue(dtReal const toSet);
      virtual void setRange(dtReal const min, dtReal const max);
      virtual void writeToElement( 
        ::QDomDocument & doc, ::QDomElement & element
      ) = 0;
      virtual dtReal getMin(void) const;
      virtual dtReal getMax(void) const;
      virtual bool loadable( void ) const;
      virtual void resolveConstraint( 
        lvH_constValue const * const cVArr
      );
    protected:
      constValue();    
    private:
      dtReal _value;
      bool _loadable;
  };
}
#endif	/* constValue_H */

