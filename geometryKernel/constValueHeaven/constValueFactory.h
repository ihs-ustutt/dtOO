#ifndef CONSTVALUEFACTORY_H
#define	CONSTVALUEFACTORY_H

#include <string>
#include <logMe/dtMacros.h>

namespace dtOO {
  class constValue;

  class constValueFactory {
    public:
      dt__CLASSNAME(constValueFactory);    
      constValueFactory();
      virtual ~constValueFactory();
      constValue * create(char const * const str) const ;
      constValue * create(std::string const str) const ;
    private:

  };
}
#endif	/* CONSTVALUEFACTORY_H */

