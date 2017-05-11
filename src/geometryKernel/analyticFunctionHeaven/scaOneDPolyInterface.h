#ifndef scaOneDPolyInterface_H
#define scaOneDPolyInterface_H

#include "scaOneD.h"

namespace dtOO {
  class dtTransformer;
  
  class scaOneDPolyInterface : public virtual scaOneD {
  public:
    dt__class(scaOneDPolyInterface, analyticFunction);
    scaOneDPolyInterface();
    scaOneDPolyInterface(scaOneDPolyInterface const & orig);    
    virtual ~scaOneDPolyInterface();
    virtual int nDOF( void ) const = 0;
    virtual void setDOF( std::vector< float > const value ) = 0;
  private:

  };
}
#endif /* scaOneDPolyInterface_H */

