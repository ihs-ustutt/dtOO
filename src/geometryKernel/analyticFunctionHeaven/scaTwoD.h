#ifndef SCATWOD_H
#define	SCATWOD_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include "scaFunction.h"

namespace dtOO {
  class renderInterface;
  class dtTransformer;
  
  class scaTwoD : public scaFunction {
  public:
    dt__class(scaTwoD, analyticFunction); 
    scaTwoD();
    scaTwoD( scaTwoD const & orig);  
    virtual ~scaTwoD();
    virtual scaTwoD * clone( void ) const = 0;
    virtual scaTwoD * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const = 0;        
    virtual scaTwoD * create( void ) const = 0;       
    virtual dtReal YFloat(aFX const & xx) const;
    virtual dtReal YFloat(dtReal const & x0, dtReal const & x1) const = 0;
    dtReal YFloatPercent( dtReal const & xP0, dtReal const & xP1 ) const;
    void setMin(int const & dir, dtReal const & min);
    void setMax(int const & dir, dtReal const & max);
    virtual int xDim( void ) const;
    virtual dtReal xMin( int const & dir) const;
    virtual dtReal xMax( int const & dir) const;   
	  using analyticFunction::x_percent;
    using analyticFunction::percent_x;
    aFX x_percent(dtReal const & x0, dtReal const & x1) const;
    aFX percent_x(dtReal const & x0, dtReal const & x1) const;
	  std::vector<dtReal> DYFloat( aFX const & xx ) const;
	  std::vector<dtReal> DYFloat( dtReal const & x0, dtReal const & x1 ) const;
    vectorHandling< renderInterface * > getRender( void ) const;
  private:
    dtReal _min[2];
    dtReal _max[2];  
  };
  dt__H_addCloneForpVH(scaTwoD);    
}
#endif	/* SCATWOD_H */

