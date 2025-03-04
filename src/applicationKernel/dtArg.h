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

#ifndef DTARG_H
#define DTARG_H

#include <dtOOTypeDef.h>

#include <boost/program_options.hpp>
#include <logMe/dtMacros.h>
namespace dtPO = boost::program_options;
typedef dtPO::options_description dtArgDesc;
typedef dtPO::variables_map dtArgMap;

namespace dtOO {
class dtArg : public dtArgMap {
public:
  dt__classOnlyName(dtArg);
  dtArg(std::string const &appName, dtInt argC, char const *const argV[]);
  virtual ~dtArg();
  void setMachine(void);
  dtArgDesc &description(void);
  void update(void);
  std::string callCommand(void);

private:
  dtArgDesc _desc;
  dtInt _argC;
  char const *const *_argV;
};
} // namespace dtOO

#endif /* DTARG_H */
