#ifndef SCAMUPARSERTWOD_H
#define	SCAMUPARSERTWOD_H

#include "scaTwoD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace mu {
  class Parser;
}

namespace dtOO {
  class scaMuParserTwoD : public scaTwoD {
  public:
    dt__CLASSSTD(scaMuParserTwoD, analyticFunction);     
    scaMuParserTwoD();
    scaMuParserTwoD(scaMuParserTwoD const & orig);
    scaMuParserTwoD(
      std::string const expression, 
      std::string const argOne, 
      std::string const argTwo
    );
    scaMuParserTwoD * clone( void ) const;
    scaMuParserTwoD * create( void ) const;    
    virtual ~scaMuParserTwoD();
    virtual float YFloat( float const & x0, float const & x1 ) const;
    virtual bool closed( int const & dir ) const;    
  private:
    mu::Parser * _parser;
    double * _arg[2];
    std::string _expressionStr;
    std::string _argStr[2];
  };
}
#endif	/* SCAMUPARSERTWOD_H */