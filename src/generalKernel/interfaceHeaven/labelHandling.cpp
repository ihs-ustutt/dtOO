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

#include "labelHandling.h"

#include <jsonHeaven/jsonPrimitive.h>
#include <logMe/logMe.h>

namespace dtOO {
labelHandling::labelHandling()
{
  std::ostringstream os;
  os << std::hex << static_cast<void *>(this);
  _label = os.str();
}

labelHandling::labelHandling(const labelHandling &orig)
{
  _label = orig._label;
}

labelHandling::labelHandling(std::string const &label) { _label = label; }

void labelHandling::jInit(jsonPrimitive const &jE)
{
  if (jE.contains("label"))
  {
    this->setLabel(jE.lookup<std::string>("label"));
  }
}

labelHandling::~labelHandling() { _label.clear(); }

void labelHandling::setLabel(std::string const label) { _label = label; }

std::string labelHandling::getLabel(void) const { return _label; }
} // namespace dtOO
