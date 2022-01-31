#ifndef EGSL_H
#define	EGSL_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <string>
#include <vector>
#include <sstream>
#include "eGeneral.h"

namespace dtOO {
  class eGSL : public eGeneral {
  public:
    dt__classOnlyName(eGSL);    
    eGSL() _GLIBCXX_USE_NOEXCEPT;
    eGSL( std::ostream & msg) _GLIBCXX_USE_NOEXCEPT;
    virtual ~eGSL() _GLIBCXX_USE_NOEXCEPT;
  };
}
#endif	/* EGSL_H */

