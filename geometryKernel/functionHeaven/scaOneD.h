#ifndef SCAONED_H
#define	SCAONED_H

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include "scaFunction.h"

namespace dtOO {
  class scaOneD : public scaFunction {
  public:
    dt__CLASSSTD(scaOneD, analyticFunction); 
    scaOneD();
    scaOneD( scaOneD const & orig);  
    virtual ~scaOneD();
    virtual scaOneD * clone( void ) const = 0;
    virtual scaOneD * create( void ) const = 0;       
    virtual float YFloat(aFX const & xx) const;
    virtual float YFloat(float const & xx) const = 0;
    float YFloatPercent( float const & xP ) const;
    void setMinMax( float const min, float const max);
    void setMin(float const min);
    void setMax(float const max);
    virtual int xDim( void ) const;
    virtual float xMin( int const & dir) const;
    virtual float xMax( int const & dir) const;   
    float getLength( void ) const;
    float x_percent(float const & xx) const;
    float percent_x(float const & xx) const;
    float DYFloat( float const & xx ) const;    
//  private:
//    float simpleNewton( float const yy) const;
  private:
    float _xxMin;
    float _xxMax;    
  };
}
#endif	/* SCAONED_H */

