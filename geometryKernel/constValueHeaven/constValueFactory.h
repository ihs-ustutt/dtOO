#ifndef CONSTVALUEFACTORY_H
#define	CONSTVALUEFACTORY_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class constValue;

  class constValueFactory {
    public:
      dt__classOnlyName(constValueFactory);    
      constValueFactory();
      virtual ~constValueFactory();
      static constValue * create(char const * const str) ;
      static constValue * create(std::string const str) ;
    private:

  };
}
#endif	/* CONSTVALUEFACTORY_H */

