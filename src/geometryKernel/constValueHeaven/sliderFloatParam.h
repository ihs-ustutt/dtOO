#ifndef SLIDERFLOATPARAM_H
#define	SLIDERFLOATPARAM_H

#include <dtOOTypeDef.h>

#include "constValue.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class sliderFloatParam : public constValue {
  public:
    dt__class(sliderFloatParam, constValue);    
    sliderFloatParam();
    sliderFloatParam(sliderFloatParam const & orig);
    virtual ~sliderFloatParam();
    virtual sliderFloatParam * clone( void ) const;
    virtual void dump(void) const;
    virtual void setRange(dtReal const min, dtReal const max);
    virtual dtReal getMin(void) const;
    virtual dtReal getMax(void) const;
    virtual void writeToElement(
      ::QDomDocument & doc, ::QDomElement & element
    );
  private:
    dtReal _min;
    dtReal _max;
  };
}
#endif	/* SLIDERFLOATPARAM_H */

