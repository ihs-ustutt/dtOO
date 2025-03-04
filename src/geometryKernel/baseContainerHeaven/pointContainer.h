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

#ifndef POINTCONTAINER_H
#define POINTCONTAINER_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
class pointContainer {
public:
  dt__classOnlyName(pointContainer);
  pointContainer();
  pointContainer(const pointContainer &orig);
  virtual ~pointContainer();
  void add(dtPoint3 const point, std::string const string);
  dtPoint3 get(std::string const string) const;
  dtPoint3 operator[](std::string const label) const;
  bool has(std::string const string) const;
  bool isEmpty(void) const;
  std::vector<std::string> labels(void) const;

private:
  std::vector<std::pair<dtPoint3, std::string>> _pair;
};
} // namespace dtOO
#endif /* POINTCONTAINER_H */
