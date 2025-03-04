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

#ifndef OUTPUT2FILE_H
#define OUTPUT2FILE_H

#include <dtOOTypeDef.h>

#include "logBase.h"
#include <fstream>

namespace dtOO {
class Output2FILE {
public:
  static std::ofstream &Stream();
  static void Output(const std::string &msg, TLogLevel level);
  static void OutputFile(const std::string &msg, TLogLevel level);

private:
  Output2FILE();
};
} // namespace dtOO
#endif /* OUTPUT2FILE_H */
