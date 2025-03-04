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

#ifndef LABELHANDLING_H
#define LABELHANDLING_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
class jsonPrimitive;

class labelHandling {
public:
  dt__classOnlyName(labelHandling);
  labelHandling();
  labelHandling(const labelHandling &orig);
  labelHandling(std::string const &label);
  void jInit(jsonPrimitive const &jE);
  virtual ~labelHandling();
  void setLabel(std::string const label);
  std::string getLabel(void) const;

private:
  std::string _label;
};
} // namespace dtOO
#endif /* LABELHANDLING_H */
