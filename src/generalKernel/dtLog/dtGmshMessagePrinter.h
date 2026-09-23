/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >

    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in
    the dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef __dtGmshMessagePrinter_H__
#define __dtGmshMessagePrinter_H__

#include <gmsh/GmshMessage.h>

namespace dtOO {

class dtGmshMessagePrinter : public GmshMessage {
public:
  dtGmshMessagePrinter();
  virtual ~dtGmshMessagePrinter();
  virtual void operator()(std::string level, std::string message) override;
  static void registerPrinter();
};

} // namespace dtOO

#endif // __dtGmshMessagePrinter_H__
