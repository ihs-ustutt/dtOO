#ifndef scaTanhUnitGradingOneD_H
#define	scaTanhUnitGradingOneD_H

#include <dtOOTypeDef.h>

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
        std::vector< dtReal > cc,               
        dtReal const & gg, dtReal const & ggMin, dtReal const & ggMax
      );
      scaTanhUnitGradingOneD * clone( void ) const;
      scaTanhUnitGradingOneD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;        
      scaTanhUnitGradingOneD * create( void ) const;    
      virtual ~scaTanhUnitGradingOneD();
      virtual dtReal YFloat(dtReal const & xx) const;  
      virtual int nDOF( void ) const;
      virtual void setDOF( std::vector< dtReal > const value );    
    private:
      dtReal _gg;
      dtReal _ggMin;
      dtReal _ggMax;
      std::vector< dtReal > _cc;
  };
  typedef 
    analyticFunctionCompound< scaTanhUnitGradingOneD >
    scaTanhUnitGradingOneDCompound;
}
#endif	/* scaTanhUnitGradingOneD_H */