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

#ifndef aFX_H
#define aFX_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class aFY;

class aFX : public std::vector<dtReal> {
public:
  aFX(void);
  aFX(const aFX &orig);
  aFX(const aFY &orig);
  aFX(std::vector<dtReal> const &orig);
  aFX(dtInt const &ss, double const &ref);
  std::vector<dtReal> toStdVector(void);
};
} // namespace dtOO
#endif /* aFX_H */
