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

#ifndef BITHICKNESSINCREASING_H
#define	BITHICKNESSINCREASING_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class scaOneD;
  
  class biThicknessIncreasing : public dtTransformer {
    public:    
      dt__classOnlyName(biThicknessIncreasing);
      biThicknessIncreasing();
      virtual ~biThicknessIncreasing();
      virtual dtTransformer * clone( void ) const;
      virtual dtTransformer * create( void ) const;     
      virtual bool isNecessary( void ) const;
      virtual void init( 
        ::QDomElement const * tE, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG 
      );
      virtual lvH_analyticFunction 
      apply( lvH_analyticFunction const * const sFunP ) const;  
      virtual void handleInt(std::string const name, dtInt const value);
      virtual void handleAnalyticFunction(
        std::string const name, analyticFunction const * value
      );
      virtual void handleBool(std::string const name, bool const value);
    private:
      scaOneD const * _tD[2];
      scaOneD const * _para;
      dtInt _nPointsOne;
      dtInt _splineOrder;
      bool _reverse;
      static bool _registrated;    
  };
}    

#endif	/* THICKNESSINCREASING_H */
