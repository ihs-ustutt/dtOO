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

#include "vectorContainer.h"
#include <logMe/logMe.h>
#include <math.h>
#include <vector>

namespace dtOO {
vectorContainer::vectorContainer() {}

vectorContainer::vectorContainer(const vectorContainer &orig) {}

vectorContainer::~vectorContainer() { _pair.clear(); }

void vectorContainer::add(dtVector3 const vector, std::string const string)
{
  add(vector, string, dtPoint3(0, 0, 0));
}

void vectorContainer::add(
  dtVector3 const vector, std::string const string, dtPoint3 const point
)
{
  if (string != "")
  {
    for (int ii = 0; ii < _pair.size(); ii++)
    {
      if (_pair[ii].second == string)
      {
        dt__throw(
          add(),
          << "Try to add a vector that is already in container." << std::endl
          << dt__eval(string)
        );
      }
    }
  }
  std::pair<dtVector3, dtPoint3> inpair(vector, point);
  _pair.push_back(
    std::pair<std::pair<dtVector3, dtPoint3>, std::string>(inpair, string)
  );
}

dtVector3 vectorContainer::get(std::string const string) const
{
  for (int ii = 0; ii < _pair.size(); ii++)
  {
    if (_pair[ii].second == string)
    {
      return (_pair[ii].first.first);
    }
  }
  dt__throw(get(), << string << " not in vectorContainer.");
}

dtVector3 vectorContainer::operator[](std::string const label) const
{
  return this->get(label);
}

bool vectorContainer::has(std::string const string) const
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

bool vectorContainer::isEmpty(void) const { return _pair.empty(); }

std::vector<std::string> vectorContainer::labels(void) const
{
  std::vector<std::string> ret;
  dt__forAllRefAuto(_pair, aPair) ret.push_back(aPair.second);
  return ret;
}
} // namespace dtOO
