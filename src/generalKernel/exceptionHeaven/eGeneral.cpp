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

#include "eGeneral.h"
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>

namespace dtOO {
eGeneral::eGeneral() _GLIBCXX_USE_NOEXCEPT : std::exception() {}

eGeneral::eGeneral(std::ostream &msg) _GLIBCXX_USE_NOEXCEPT : std::exception()
{
  std::ostringstream &s = dynamic_cast<std::ostringstream &>(msg);
  _exceptionMsg = s.str();
}

eGeneral::~eGeneral() _GLIBCXX_USE_NOEXCEPT {}

void eGeneral::clear(void)
{
  _exceptionMsg.clear();
  std::cout << "_exceptionMsg = " << _exceptionMsg << std::endl;
}

const char *eGeneral::what(void) const _GLIBCXX_USE_NOEXCEPT
{
  return _exceptionMsg.c_str();
}
} // namespace dtOO
