#ifndef scaOneDPolyInterface_H
#define scaOneDPolyInterface_H

#include <dtOOTypeDef.h>

#include "scaOneD.h"

namespace dtOO {
  class dtTransformer;
  
  class scaOneDPolyInterface : public scaOneD {
    public:
      dt__class(scaOneDPolyInterface, analyticFunction);
      scaOneDPolyInterface();
      scaOneDPolyInterface(scaOneDPolyInterface const & orig);    
      virtual scaOneDPolyInterface * clone( void ) const = 0;
      virtual ~scaOneDPolyInterface();
      virtual dtInt nDOF( void ) const = 0;
      virtual void setDOF( std::vector< dtReal > const value ) = 0;
  };
  dt__H_addCloneForpVH(scaOneDPolyInterface);
}
#endif /* scaOneDPolyInterface_H */

