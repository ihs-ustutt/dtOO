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

#include "pointContainer.h"
#include "interfaceHeaven/ptrHandling.h"
#include <discrete3dPoints.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/logMe.h>
#include <vector>

namespace dtOO {
pointContainer::pointContainer() {}

pointContainer::pointContainer(const pointContainer &orig) {}

pointContainer::~pointContainer() { _pair.clear(); }

void pointContainer::add(dtPoint3 const point, std::string const string)
{
  if (string != "")
  {
    for (int ii = 0; ii < _pair.size(); ii++)
    {
      if (_pair[ii].second == string)
      {
        dt__throw(
          add(),
          << "Try to add a point that is already in container." << std::endl
          << dt__eval(string)
        );
      }
    }
  }
  dt__debug(add(), << "adding point " << string);

  _pair.push_back(std::pair<dtPoint3, std::string>(point, string));
}

dtPoint3 pointContainer::get(std::string const string) const
{
  for (int ii = 0; ii < _pair.size(); ii++)
  {
    if (_pair[ii].second == string)
    {
      return (_pair[ii].first);
    }
  }
  dt__throw(get(), << string << " not in pointContainer.");
}

dtPoint3 pointContainer::operator[](std::string const label) const
{
  return this->get(label);
}

bool pointContainer::has(std::string const string) const
{
  for (int ii = 0; ii < _pair.size(); ii++)
  {
    if (_pair[ii].second == string)
    {
      return true;
    }
  }
  return false;
}

bool pointContainer::isEmpty(void) const { return _pair.empty(); }

std::vector<std::string> pointContainer::labels(void) const
{
  std::vector<std::string> ret;
  dt__forAllRefAuto(_pair, aPair) ret.push_back(aPair.second);
  return ret;
}
} // namespace dtOO
