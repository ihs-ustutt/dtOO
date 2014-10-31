#ifndef DERIVABLE_H
#define	DERIVABLE_H

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
  class derivable {
  public:
    dt__CLASSNAME(derivable);
    derivable();
    derivable(const derivable& orig);
    virtual ~derivable();
  public:
    virtual std::vector< float > getVectorNormal(float const xx) const = 0;
    virtual std::vector< float > getVectorNormalPercent(float const xx) const = 0;
    virtual float getLength( void ) const = 0;
  private:

  };
}
#endif	/* DERIVABLE_H */

