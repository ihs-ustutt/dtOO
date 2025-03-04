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

#include "logBase.h"

#include <sys/time.h>
// #include <sstream>
// #include <string>

namespace dtOO {
std::string NowTime()
{
  char buffer[11];
  time_t t;
  time(&t);
  tm r = {0};
  strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
  struct timeval tv;
  gettimeofday(&tv, 0);
  char result[100] = {0};
  sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
  return result;
}

std::string NowDateAndTime()
{
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%d-%m-%Y.%X", &tstruct);

  return buf;
}
} // namespace dtOO
