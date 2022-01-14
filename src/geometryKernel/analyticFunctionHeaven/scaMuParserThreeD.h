#ifndef scaMuParserThreeD_H
#define	scaMuParserThreeD_H

#include <dtOOTypeDef.h>

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
    virtual dtReal YFloat( 
      dtReal const & x0, dtReal const & x1, dtReal const & x2 
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