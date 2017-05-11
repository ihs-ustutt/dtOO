#ifndef scaSingleTanhGradingOneD_H
#define	scaSingleTanhGradingOneD_H

#include "scaOneD.h"
#include "scaOneDPolyInterface.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunctionCompound.h"

namespace dtOO {
  class dtTransformer;
  
  class scaSingleTanhGradingOneD 
    : public virtual scaOneD, public scaOneDPolyInterface {
    public:
      dt__class(scaSingleTanhGradingOneD, analyticFunction);     
      scaSingleTanhGradingOneD();
      scaSingleTanhGradingOneD(scaSingleTanhGradingOneD const & orig);
      scaSingleTanhGradingOneD( 
        float const & gg, float const & ggMin, float const & ggMax
      );
      scaSingleTanhGradingOneD * clone( void ) const;
      scaSingleTanhGradingOneD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;        
      scaSingleTanhGradingOneD * create( void ) const;    
      virtual ~scaSingleTanhGradingOneD();
      virtual float YFloat(float const & xx) const;  
      virtual int nDOF( void ) const;
      virtual void setDOF( std::vector< float > const value );    
    private:
      float _gg;
      float _ggMin;
      float _ggMax;
  };
  typedef 
    analyticFunctionCompound< scaSingleTanhGradingOneD >
    scaSingleTanhGradingOneDCompound;
}
#endif	/* scaSingleTanhGradingOneD_H */