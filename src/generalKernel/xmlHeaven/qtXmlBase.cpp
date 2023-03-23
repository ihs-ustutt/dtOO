#include "qtXmlBase.h"

#include <logMe/logMe.h>
#include <parseHeaven/dtParser.h>
#include <interfaceHeaven/intHandling.h>

namespace dtOO {
  qtXmlBase::qtXmlBase() {
  }

  qtXmlBase::~qtXmlBase() {
  }

  dtReal qtXmlBase::muParseString( std::string const expression ) {
    return dtParser()(expression)[0];
  }

  std::vector< dtReal > qtXmlBase::muParseCSString( 
    std::string const expression 
  ) {
    return dtParser()(expression);
  }  

  dtInt qtXmlBase::muParseStringInt( std::string const expression ) {
    return intHandling::round( dtParser()(expression)[0] );
  }  

  std::vector< dtInt > qtXmlBase::muParseCSStringInt( 
    std::string const expression 
  ) {
    std::vector< dtReal > vec = dtParser()(expression);
    std::vector< dtInt > retVec(vec.size());
    dt__forAllIndex(vec, ii) retVec[ii] = intHandling::round( vec[ii] );

    return retVec;
  }  
}
