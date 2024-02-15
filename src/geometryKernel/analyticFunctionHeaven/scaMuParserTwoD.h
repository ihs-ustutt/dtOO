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

#ifndef SCAMUPARSERTWOD_H
#define	SCAMUPARSERTWOD_H

#include <dtOOTypeDef.h>

#include "scaTwoD.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtTransformer;
  class dtMuParser;
  
  class scaMuParserTwoD : public scaTwoD {
    public:
      dt__class(scaMuParserTwoD, analyticFunction);     
      scaMuParserTwoD();
      scaMuParserTwoD(scaMuParserTwoD const & orig);
      scaMuParserTwoD(
        std::string const expression, 
        std::string const argOne, 
        std::string const argTwo
      );
      scaMuParserTwoD * clone( void ) const;
      virtual scaMuParserTwoD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;      
      scaMuParserTwoD * create( void ) const;    
      virtual ~scaMuParserTwoD();
      virtual dtReal YFloat( dtReal const & x0, dtReal const & x1 ) const;
      virtual bool closed( dtInt const & dir ) const;    
    private:
      dt__pH(dtMuParser) _parser;
      dt__pVH(double) _arg;
      std::string _expressionStr;
      std::string _argStr[2];
  };
}
#endif	/* SCAMUPARSERTWOD_H */
