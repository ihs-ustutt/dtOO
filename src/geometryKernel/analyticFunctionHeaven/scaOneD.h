#ifndef SCAONED_H
#define	SCAONED_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include "scaFunction.h"

namespace dtOO {
  class dtTransformer;
  
  class scaOneD : public scaFunction {
  public:
    dt__class(scaOneD, analyticFunction); 
    scaOneD();
    scaOneD( scaOneD const & orig);  
    virtual ~scaOneD();
    virtual scaOneD * clone( void ) const = 0;
    virtual scaOneD * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const = 0;      
    virtual scaOneD * create( void ) const = 0;       
    virtual dtReal YFloat(aFX const & xx) const;
    virtual dtReal YFloat(dtReal const & xx) const = 0;
    virtual dtReal invYFloat(dtReal const & yy) const;     
    dtReal YFloatPercent( dtReal const & xP ) const;
    void setMinMax( dtReal const min, dtReal const max);
    void setMin(dtReal const min);
    void setMax(dtReal const max);
    virtual int xDim( void ) const;
    virtual dtReal xMin( int const & dir) const;
    virtual dtReal xMax( int const & dir) const;   
    dtReal getLength( void ) const;
    dtReal x_percent(dtReal const & xx) const;
    dtReal percent_x(dtReal const & xx) const;
    dtReal DYFloat( dtReal const & xx ) const;    
    virtual vectorHandling< renderInterface * > getRender( void ) const;
//  private:
//    dtReal simpleNewton( dtReal const yy) const;
  private:
    dtReal _xxMin;
    dtReal _xxMax;    
  };
  dt__H_addCloneForpVH(scaOneD);      
}
#endif	/* SCAONED_H */

