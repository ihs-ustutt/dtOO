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

#ifndef TIMEHANDLING_H
#define TIMEHANDLING_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace boost {
namespace timer {
class cpu_timer;
}
} // namespace boost

namespace dtOO {
class timeHandling {
public:
  dt__classOnlyName(timeHandling);
  timeHandling(std::string const &keyword);
  virtual ~timeHandling();
  void output(void);

private:
  std::string _keyword;
  dt__pH(::boost::timer::cpu_timer) _t;
  dtInt _step;
};
} // namespace dtOO
#endif /* TIMEHANDLING_H */
