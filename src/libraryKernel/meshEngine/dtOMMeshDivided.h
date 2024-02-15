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

#ifndef DTOMMESHDIVIDED_H
#define	DTOMMESHDIVIDED_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

#include "dtOMMesh.h"

class MElement;
class MVertex;

namespace dtOO {
  class dtOMMeshDivided {
  public:
    dt__classOnlyName(dtOMMeshDivided);    
    dtOMMeshDivided();
    virtual ~dtOMMeshDivided();
    void add( dtOMMesh const & om );
    dtOMMesh connect( void );
  private:
    std::list< dtOMMesh > _part;
  };
}
#endif	/* DTOMMESHDIVIDED_H */
