#ifndef resultValue_H
#define	resultValue_H

#include <string>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>

namespace dtOO {
  class resultValue : public labelHandling {
  public:
    dt__class(resultValue, resultValue);    
    resultValue( resultValue const & orig );
    virtual ~resultValue();
    virtual resultValue * clone( void ) const = 0;    
    virtual float operator()( void ) = 0;
  protected:
    resultValue( std::string const & label ); 
  };
}
#endif	/* resultValue_H */

