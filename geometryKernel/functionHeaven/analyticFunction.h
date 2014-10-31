#ifndef ANALYTICFUNCTION_H
#define	ANALYTICFUNCTION_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/labelHandling.h>
#include <vector>
#include <interfaceHeaven/renderInterface.h>

namespace dtOO {
  typedef std::vector< float > aFX;
  typedef std::vector< float > aFY;
  
  class analyticFunction : public optionHandling,
                           public labelHandling, 
                           public renderInterface {
  public:
    dt__CLASSNAME(analyticFunction);    
    analyticFunction();
    analyticFunction(const analyticFunction& orig);
    virtual ~analyticFunction();
    virtual analyticFunction * clone( void ) const = 0;
    virtual analyticFunction * create( void ) const = 0;
    virtual void dump(void) const;    
    virtual aFY Y(aFX const & xx) const = 0;
    virtual int xDim( void ) const = 0;
    virtual float xMin( int const & dir) const = 0;
    virtual float xMax( int const & dir) const = 0;
  };
}
#endif	/* ANALYTICFUNCTION_H */

