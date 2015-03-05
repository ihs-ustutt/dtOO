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
    dt__CLASSSTD(scaMuParserOneD, analyticFunction);     
    scaMuParserOneD();
    scaMuParserOneD(scaMuParserOneD const & orig);
    scaMuParserOneD(std::string const expression, std::string const argument);
    scaMuParserOneD(mu::Parser * parserP, double * argumentP, float const * const xxMin, float const * const xxMax);
    scaMuParserOneD * clone( void ) const;
    scaMuParserOneD * create( void ) const;    
    virtual ~scaMuParserOneD();
    virtual float YFloat(float const & xx) const;
    void setArgumentRange(float xMin, float xMax);  
    vectorHandling< renderInterface * > getRender( void ) const;   
  private:
    mu::Parser * _parser;
    double * _argument;
    std::string _expressionStr;
    std::string _argumentStr;
//    scaMuParserOneD * _derivative;
  };
}
#endif	/* SCAMUPARSERONED_H */