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

#ifndef dtMuParser_H
#define dtMuParser_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <muParser.h>

namespace dtOO {
class dtMuParser : public ::mu::Parser {
public:
  dt__classOnlyName(dtMuParser);
  dtMuParser();
  dtMuParser(const dtMuParser &orig);
  virtual ~dtMuParser();

private:
};
} // namespace dtOO
#endif /* dtMuParser_H */
