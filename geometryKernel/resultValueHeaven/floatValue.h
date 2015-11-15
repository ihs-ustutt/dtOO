#ifndef floatValue_H
#define	floatValue_H

#include <string>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>

#include "resultValue.h"

namespace dtOO {
  class floatValue : public resultValue {
  public:
    dt__class(floatValue, resultValue);    
    floatValue( floatValue const & orig );
    floatValue( std::string const & label, float const & value  );
    virtual ~floatValue();
    virtual floatValue * clone( void ) const;    
    virtual float operator()( void );
    void operator()( float const & value );
  private:
    float _value;
  };
}
#endif	/* floatValue_H */

