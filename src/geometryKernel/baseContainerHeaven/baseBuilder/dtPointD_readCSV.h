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

#ifndef dtPointD_readCSV_H
#define dtPointD_readCSV_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtPointD_readCSV {
public:
  dt__classOnlyName(dtPointD_readCSV);
  dtPointD_readCSV(std::string const &filename);
  virtual ~dtPointD_readCSV();
  std::vector<dtPointD> result(void) const;

private:
  std::vector<dtPointD> _pp;
};
} // namespace dtOO
#endif /* dtPointD_readCSV_H */
