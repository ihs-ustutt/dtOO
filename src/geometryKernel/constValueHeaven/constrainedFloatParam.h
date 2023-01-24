#ifndef constrainedFloatParam_H
#define	constrainedFloatParam_H

#include <dtOOTypeDef.h>

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
    virtual void setRange(dtReal const min, dtReal const max);
    virtual dtReal getMin(void) const;
    virtual dtReal getMax(void) const;
    virtual dtReal getValue(void) const;
    virtual std::string getValueStr(void) const;
    virtual void setValue(dtReal const toSet);    
    virtual void writeToElement(
      ::QDomDocument & doc, ::QDomElement & element
    );
    virtual bool loadable( void ) const;    
    virtual void resolveConstraint( 
      lvH_constValue const * const cVArr
    );
  private:
    dtReal _min;
    dtReal _max;
    std::string _valueStr;
    lvH_constValue const * _cVArr;
  };
}
#endif	/* constrainedFloatParam_H */

