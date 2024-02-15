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

#ifndef intHandling_H
#define	intHandling_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
  class intHandling {
    public:
      dt__classOnlyName(intHandling);
      virtual ~intHandling();
      static dtInt round( dtReal a );
      static dtInt bound( 
        dtInt const & toBound, dtInt const & aa, dtInt const & bb 
      );
    private:
      intHandling();
  };
}
#endif	/* intHandling_H */
