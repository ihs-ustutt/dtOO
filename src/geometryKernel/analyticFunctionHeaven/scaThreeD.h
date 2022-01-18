#ifndef scaThreeD_H
#define	scaThreeD_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include "scaFunction.h"

namespace dtOO {
  class renderInterface;
  class dtTransformer;
  
  class scaThreeD : public scaFunction {
  public:
    dt__class(scaThreeD, analyticFunction); 
    scaThreeD();
    scaThreeD( scaThreeD const & orig);  
    virtual ~scaThreeD();
    virtual scaThreeD * clone( void ) const = 0;
    virtual scaThreeD * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const = 0;        
    virtual scaThreeD * create( void ) const = 0;       
    virtual dtReal YFloat(aFX const & xx) const;
    virtual dtReal YFloat(
      dtReal const & x0, dtReal const & x1, dtReal const & x2
    ) const = 0;
    dtReal YFloatPercent( dtReal const & xP0, dtReal const & xP1, dtReal const & xP2 ) const;
    void setMin(int const & dir, dtReal const & min);
    void setMax(int const & dir, dtReal const & max);
    virtual dtInt xDim( void ) const;
    virtual dtReal xMin( dtInt const & dir) const;
    virtual dtReal xMax( dtInt const & dir) const;   
	  using analyticFunction::x_percent;
    using analyticFunction::percent_x;
    aFX x_percent(dtReal const & x0, dtReal const & x1, dtReal const & x2) const;
    aFX percent_x(dtReal const & x0, dtReal const & x1, dtReal const & x2) const;
	  std::vector<dtReal> DYFloat( aFX const & xx ) const;
	  std::vector<dtReal> DYFloat( 
      dtReal const & x0, dtReal const & x1, dtReal const & x2 
    ) const;
    vectorHandling< renderInterface * > getRender( void ) const;
  private:
    dtReal _min[3];
    dtReal _max[3];  
  };
  dt__H_addCloneForpVH(scaThreeD);    
}
#endif	/* scaThreeD_H */

