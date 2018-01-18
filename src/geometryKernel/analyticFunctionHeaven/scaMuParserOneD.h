#ifndef SCAMUPARSERONED_H
#define	SCAMUPARSERONED_H

#include "scaOneD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;
  class dtMuParser;
  
  class scaMuParserOneD : public scaOneD {
  public:
    dt__class(scaMuParserOneD, analyticFunction);     
    scaMuParserOneD();
    scaMuParserOneD(scaMuParserOneD const & orig);
    scaMuParserOneD(std::string const expression, std::string const argument);
    scaMuParserOneD * clone( void ) const;
    scaMuParserOneD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;        
    scaMuParserOneD * create( void ) const;    
    virtual ~scaMuParserOneD();
    virtual float YFloat(float const & xx) const;
    void setArgumentRange(float xMin, float xMax);  
  private:
    dt__pH(dtMuParser) _parser;
    dt__pH(double) _argument;
    std::string _expressionStr;
    std::string _argumentStr;
  };
}
#endif	/* SCAMUPARSERONED_H */