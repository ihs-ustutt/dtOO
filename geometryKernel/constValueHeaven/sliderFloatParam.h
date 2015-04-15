#ifndef SLIDERFLOATPARAM_H
#define	SLIDERFLOATPARAM_H

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
    virtual void setRange(float const min, float const max);
    virtual float getMin(void) const;
    virtual float getMax(void) const;
    virtual void writeToElement(QDomDocument & doc, QDomElement & element);
  private:
    float _min;
    float _max;
  };
}
#endif	/* SLIDERFLOATPARAM_H */

