#ifndef scaTanhGradingOneD_H
#define	scaTanhGradingOneD_H

#include "scaOneD.h"
#include "scaOneDPolyInterface.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunctionCompound.h"

namespace dtOO {
  class dtTransformer;
  
  class scaTanhGradingOneD 
    : public virtual scaOneD, public scaOneDPolyInterface {
    public:
      dt__class(scaTanhGradingOneD, analyticFunction);     
      scaTanhGradingOneD();
      scaTanhGradingOneD(scaTanhGradingOneD const & orig);
      scaTanhGradingOneD( 
        std::vector< float > cc,               
        float const & gg, float const & ggMin, float const & ggMax
      );
      scaTanhGradingOneD * clone( void ) const;
      scaTanhGradingOneD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;        
      scaTanhGradingOneD * create( void ) const;    
      virtual ~scaTanhGradingOneD();
      virtual float YFloat(float const & xx) const;  
      virtual int nDOF( void ) const;
      virtual void setDOF( std::vector< float > const value );    
    private:
      float _gg;
      float _ggMin;
      float _ggMax;
      std::vector< float > _cc;
  };
  typedef 
    analyticFunctionCompound< scaTanhGradingOneD >
    scaTanhGradingOneDCompound;
}
#endif	/* scaTanhGradingOneD_H */