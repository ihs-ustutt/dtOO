#ifndef scaLinearOneD_H
#define	scaLinearOneD_H

#include <dtOOTypeDef.h>

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
        dtReal const & x0, dtReal const & x1, dtReal const & y0, dtReal const & y1
      );
      scaLinearOneD * clone( void ) const;
      scaLinearOneD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;        
      scaLinearOneD * create( void ) const;    
      virtual ~scaLinearOneD();
      virtual dtReal YFloat(dtReal const & xx) const;
      virtual dtReal invYFloat(dtReal const & yy) const;
    private:
      dtReal _m;
      dtReal _b;
  };
}
#endif	/* scaLinearOneD_H */