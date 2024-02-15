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

#ifndef floatValue_H
#define	floatValue_H

#include <dtOOTypeDef.h>

#include <string>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/labelHandling.h>

#include "resultValue.h"

namespace dtOO {
  class floatValue : public resultValue {
  public:
    dt__class(floatValue, resultValue);    
    floatValue( floatValue const & orig );
    floatValue( std::string const & label, dtReal const & value  );
    virtual ~floatValue();
    virtual floatValue * clone( void ) const;    
    virtual dtReal operator()( void );
    void operator()( dtReal const & value );
  private:
    dtReal _value;
  };
}
#endif	/* floatValue_H */
