#ifndef floatValue_H
#define	floatValue_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/labelHandling.h>

#include "resultValue.h"

namespace dtOO {
  class floatValue : public resultValue {
  public:
    dt__class(floatValue, resultValue);    
    floatValue( floatValue const & orig );
    floatValue( std::string const & label, dtReal const & value  );
    virtual ~floatValue();
    virtual floatValue * clone( void ) const;    
    virtual dtReal operator()( void );
    void operator()( dtReal const & value );
  private:
    dtReal _value;
  };
}
#endif	/* floatValue_H */

