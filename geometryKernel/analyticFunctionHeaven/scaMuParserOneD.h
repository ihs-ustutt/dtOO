#ifndef SCAMUPARSERONED_H
#define	SCAMUPARSERONED_H

#include "scaOneD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace mu {
  class Parser;
}

namespace dtOO {
  class scaMuParserOneD : public scaOneD {
  public:
    dt__class(scaMuParserOneD, analyticFunction);     
    scaMuParserOneD();
    scaMuParserOneD(scaMuParserOneD const & orig);
    scaMuParserOneD(std::string const expression, std::string const argument);
    scaMuParserOneD * clone( void ) const;
    scaMuParserOneD * create( void ) const;    
    virtual ~scaMuParserOneD();
    virtual float YFloat(float const & xx) const;
    void setArgumentRange(float xMin, float xMax);  
    vectorHandling< renderInterface * > getRender( void ) const;   
  private:
    dt__pH(mu::Parser) _parser;
    dt__pH(double) _argument;
    std::string _expressionStr;
    std::string _argumentStr;
//    scaMuParserOneD * _derivative;
  };
}
#endif	/* SCAMUPARSERONED_H */