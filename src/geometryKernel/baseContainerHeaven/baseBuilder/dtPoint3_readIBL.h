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

#ifndef dtPoint3_readIBL_H
#define	dtPoint3_readIBL_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtPoint3_readIBL {
  public:
    dt__classOnlyName(dtPoint3_readIBL);
    dtPoint3_readIBL(std::string const & filename);
    virtual ~dtPoint3_readIBL();
    std::vector< dtPoint3 > result(void) const;
  private:
    std::vector< dtPoint3 > _pp;
  };
}
#endif	/* dtPoint3_readIBL_H */
