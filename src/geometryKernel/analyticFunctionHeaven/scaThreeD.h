#ifndef scaThreeD_H
#define	scaThreeD_H

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
    virtual float YFloat(aFX const & xx) const;
    virtual float YFloat(
      float const & x0, float const & x1, float const & x2
    ) const = 0;
    float YFloatPercent( float const & xP0, float const & xP1, float const & xP2 ) const;
    void setMin(int const & dir, float const & min);
    void setMax(int const & dir, float const & max);
    virtual int xDim( void ) const;
    virtual float xMin( int const & dir) const;
    virtual float xMax( int const & dir) const;   
	  using analyticFunction::x_percent;
    using analyticFunction::percent_x;
    aFX x_percent(float const & x0, float const & x1, float const & x2) const;
    aFX percent_x(float const & x0, float const & x1, float const & x2) const;
	  std::vector<float> DYFloat( aFX const & xx ) const;
	  std::vector<float> DYFloat( 
      float const & x0, float const & x1, float const & x2 
    ) const;
    vectorHandling< renderInterface * > getRender( void ) const;
  private:
    float _min[3];
    float _max[3];  
  };
  dt__H_addCloneForpVH(scaThreeD);    
}
#endif	/* scaThreeD_H */

