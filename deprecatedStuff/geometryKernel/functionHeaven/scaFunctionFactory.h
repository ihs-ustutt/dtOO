#ifndef SCAFUNCTIONFACTORY_H
#define	SCAFUNCTIONFACTORY_H

#include <string>
#include <string.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

using namespace std;

namespace dtOO {
  class scaFunction;
  
  class scaFunctionFactory {
  public:
    dt__CLASSNAME(scaFunctionFactory);    
    scaFunctionFactory();
    ~scaFunctionFactory();
    scaFunction * create(char const * const str) const;
    scaFunction * create(string const str) const;
  private:

  };
}
#endif	/* SCAFUNCTIONFACTORY_H */

