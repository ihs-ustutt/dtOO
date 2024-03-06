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

#ifndef dtPoint3_readCSV_H
#define	dtPoint3_readCSV_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtPoint3_readCSV {
  public:
    dt__classOnlyName(dtPoint3_readCSV);
    dtPoint3_readCSV(std::string const & filename);
    virtual ~dtPoint3_readCSV();
    std::vector< dtPoint3 > result(void) const;
  private:
    std::vector< dtPoint3 > _pp;
  };
}
#endif	/* dtPoint3_readCSV_H */
