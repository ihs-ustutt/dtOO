/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef scaTanhGradingOneD_H
#define	scaTanhGradingOneD_H

#include <dtOOTypeDef.h>

#include "scaOneD.h"
#include "scaOneDPolyInterface.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunctionCompound.h"

namespace dtOO {
  class dtTransformer;
  
  class scaTanhGradingOneD : public scaOneDPolyInterface {
    public:
      dt__class(scaTanhGradingOneD, analyticFunction);     
      scaTanhGradingOneD();
      scaTanhGradingOneD(scaTanhGradingOneD const & orig);
      scaTanhGradingOneD( 
        std::vector< dtReal > cc,               
        dtReal const & gg, dtReal const & ggMin, dtReal const & ggMax
      );
      scaTanhGradingOneD * clone( void ) const;
      scaTanhGradingOneD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;        
      scaTanhGradingOneD * create( void ) const;    
      virtual ~scaTanhGradingOneD();
      virtual dtReal YFloat(dtReal const & xx) const;  
      virtual dtInt nDOF( void ) const;
      virtual void setDOF( std::vector< dtReal > const value );    
    private:
      dtReal _gg;
      dtReal _ggMin;
      dtReal _ggMax;
      std::vector< dtReal > _cc;
  };
  typedef 
    analyticFunctionCompound< scaTanhGradingOneD >
    scaTanhGradingOneDCompound;
}
#endif	/* scaTanhGradingOneD_H */
