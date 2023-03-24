#ifndef SLIDERFLOATPARAM_H
#define	SLIDERFLOATPARAM_H

#include <dtOOTypeDef.h>

#include "constValue.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class sliderFloatParam : public constValue {
    public:
      dt__class(sliderFloatParam, constValue);    
      sliderFloatParam(sliderFloatParam const & orig);
      sliderFloatParam(
        std::string const & label, dtReal const & val
      );
      sliderFloatParam(
        std::string const & label, 
        dtReal const & val, dtReal const & min, dtReal const & max
      );
      virtual ~sliderFloatParam();
      virtual sliderFloatParam * clone( void ) const;
      virtual void dump(void) const;
      virtual void writeToElement(
        ::QDomDocument & doc, ::QDomElement & element
      );
  };
}
#endif	/* SLIDERFLOATPARAM_H */

