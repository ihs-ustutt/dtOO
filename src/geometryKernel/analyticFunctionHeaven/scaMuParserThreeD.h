#ifndef scaMuParserThreeD_H
#define	scaMuParserThreeD_H

#include "scaThreeD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;
  class dtMuParser;
  
  class scaMuParserThreeD : public scaThreeD {
  public:
    dt__class(scaMuParserThreeD, analyticFunction);     
    scaMuParserThreeD();
    scaMuParserThreeD(scaMuParserThreeD const & orig);
    scaMuParserThreeD(
      std::string const expression, 
      std::string const argOne, 
      std::string const argTwo,
      std::string const argThree    
    );
    scaMuParserThreeD * clone( void ) const;
    virtual scaMuParserThreeD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;      
    scaMuParserThreeD * create( void ) const;    
    virtual ~scaMuParserThreeD();
    virtual float YFloat( 
      float const & x0, float const & x1, float const & x2 
    ) const;
    virtual bool closed( int const & dir ) const;    
  private:
    dt__pH(dtMuParser) _parser;
    dt__pVH(double) _arg;
    std::string _expressionStr;
    std::string _argStr[3];
  };
}
#endif	/* scaMuParserThreeD_H */