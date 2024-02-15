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

#ifndef resultValue_H
#define	resultValue_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>

namespace dtOO {
  class resultValue : public labelHandling {
    public:
      dt__class(resultValue, resultValue);    
      resultValue( resultValue const & orig );
      virtual ~resultValue();
      virtual resultValue * clone( void ) const = 0;    
      virtual dtReal operator()( void ) = 0;
    protected:
      resultValue( std::string const & label ); 
  };
  dt__I_addCloneForpVHNotImpl(resultValue);
}
#endif	/* resultValue_H */
