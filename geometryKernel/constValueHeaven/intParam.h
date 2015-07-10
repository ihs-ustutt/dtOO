#ifndef INTPARAM_H
#define	INTPARAM_H

#include "constValue.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class intParam : public constValue {
  public:
    dt__class(intParam, constValue);        
    intParam();
    intParam(intParam const & orig);
    virtual ~intParam();
    virtual intParam * clone( void ) const;    
    virtual void dump(void) const;
    virtual void writeToElement(QDomDocument & doc, ::QDomElement & element);
  private:
  };
}
#endif	/* INTPARAM_H */

