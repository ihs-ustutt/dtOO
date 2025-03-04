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

#ifndef EGSL_H
#define EGSL_H

#include <dtOOTypeDef.h>

#include "eGeneral.h"
#include <logMe/dtMacros.h>
#include <sstream>
#include <string>
#include <vector>

namespace dtOO {
class eGSL : public eGeneral {
public:
  dt__classOnlyName(eGSL);
  eGSL() _GLIBCXX_USE_NOEXCEPT;
  eGSL(std::ostream &msg) _GLIBCXX_USE_NOEXCEPT;
  virtual ~eGSL() _GLIBCXX_USE_NOEXCEPT;
};
} // namespace dtOO
#endif /* EGSL_H */
