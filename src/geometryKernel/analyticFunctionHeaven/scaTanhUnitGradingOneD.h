#ifndef scaTanhUnitGradingOneD_H
#define	scaTanhUnitGradingOneD_H

#include "scaOneD.h"
#include "scaOneDPolyInterface.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunctionCompound.h"

namespace dtOO {
  class dtTransformer;
  
  class scaTanhUnitGradingOneD 
    : public virtual scaOneD, public scaOneDPolyInterface {
    public:
      dt__class(scaTanhUnitGradingOneD, analyticFunction);     
      scaTanhUnitGradingOneD();
      scaTanhUnitGradingOneD(scaTanhUnitGradingOneD const & orig);
      scaTanhUnitGradingOneD( 
        std::vector< float > cc,               
        float const & gg, float const & ggMin, float const & ggMax
      );
      scaTanhUnitGradingOneD * clone( void ) const;
      scaTanhUnitGradingOneD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;        
      scaTanhUnitGradingOneD * create( void ) const;    
      virtual ~scaTanhUnitGradingOneD();
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
    analyticFunctionCompound< scaTanhUnitGradingOneD >
    scaTanhUnitGradingOneDCompound;
}
#endif	/* scaTanhUnitGradingOneD_H */