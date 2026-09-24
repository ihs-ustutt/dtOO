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

#ifndef __LOGME_H__
#define __LOGME_H__

#include <dtOOTypeDef.h>

#include "dtMacros.h"
#include <dtLog/dtLog.h>
#include <logMe/dtMacros.h>
#include <string>

namespace dtOO {
class logMe {
public:
  dt__classOnlyName(logMe);
  static std::string initLog(std::string const &logFileName);
};

} // namespace dtOO
#endif //__LOGME_H__
