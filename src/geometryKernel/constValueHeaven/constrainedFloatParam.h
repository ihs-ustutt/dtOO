#ifndef constrainedFloatParam_H
#define	constrainedFloatParam_H

#include "constValue.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class constrainedFloatParam : public constValue {
  public:
    dt__class(constrainedFloatParam, constValue);    
    constrainedFloatParam( std::string const & valueStr );
    constrainedFloatParam(constrainedFloatParam const & orig);
    virtual ~constrainedFloatParam();
    virtual constrainedFloatParam * clone( void ) const;
    virtual void dump(void) const;
    virtual void setRange(float const min, float const max);
    virtual float getMin(void) const;
    virtual float getMax(void) const;
    virtual float getValue(void) const;
    virtual void setValue(float const toSet);    
    virtual void writeToElement(
      ::QDomDocument & doc, ::QDomElement & element
    );
    virtual bool loadable( void ) const;    
    virtual void resolveConstraint( 
      cVPtrVec const * const cVArr
    );
  private:
    float _min;
    float _max;
    std::string _valueStr;
    cVPtrVec const * _cVArr;
  };
}
#endif	/* constrainedFloatParam_H */

