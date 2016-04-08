#ifndef scaLinearOneD_H
#define	scaLinearOneD_H

#include "scaOneD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;
  
  class scaLinearOneD : public scaOneD {
    public:
      dt__class(scaLinearOneD, analyticFunction);     
      scaLinearOneD();
      scaLinearOneD(scaLinearOneD const & orig);
      scaLinearOneD(
        float const & x0, float const & x1, float const & y0, float const & y1
      );
      scaLinearOneD * clone( void ) const;
      scaLinearOneD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;        
      scaLinearOneD * create( void ) const;    
      virtual ~scaLinearOneD();
      virtual float YFloat(float const & xx) const;
      virtual float invYFloat(float const & yy) const;
    private:
      float _m;
      float _b;
  };
}
#endif	/* scaLinearOneD_H */