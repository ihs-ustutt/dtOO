#ifndef SCATWOD_H
#define	SCATWOD_H

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
    virtual float YFloat(aFX const & xx) const;
    virtual float YFloat(float const & x0, float const & x1) const = 0;
    float YFloatPercent( float const & xP0, float const & xP1 ) const;
    void setMin(int const & dir, float const & min);
    void setMax(int const & dir, float const & max);
    virtual int xDim( void ) const;
    virtual float xMin( int const & dir) const;
    virtual float xMax( int const & dir) const;   
	  using analyticFunction::x_percent;
    using analyticFunction::percent_x;
    aFX x_percent(float const & x0, float const & x1) const;
    aFX percent_x(float const & x0, float const & x1) const;
	  std::vector<float> DYFloat( aFX const & xx ) const;
	  std::vector<float> DYFloat( float const & x0, float const & x1 ) const;
    vectorHandling< renderInterface * > getRender( void ) const;
  private:
    float _min[2];
    float _max[2];  
  };
  dt__H_addCloneForpVH(scaTwoD);    
}
#endif	/* SCATWOD_H */

