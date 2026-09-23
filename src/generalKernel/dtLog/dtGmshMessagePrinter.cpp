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

#include "dtGmshMessagePrinter.h"

#include "dtLog.h"

namespace dtOO {

dtGmshMessagePrinter::dtGmshMessagePrinter() : GmshMessage() {}

dtGmshMessagePrinter::~dtGmshMessagePrinter() {}

void dtGmshMessagePrinter::operator()(std::string level, std::string message)
{
  if (level == "Error")
    dtLog__error << message;
  else if (level == "Warning")
    dtLog__warning << message;
  else if (level == "Info")
    dtLog__info << message;
  else if (level == "Debug")
    dtLog__debug << message;
  else
  {
    dtLog__trace << "Gmsh message with level = " << level;
    dtLog__info << message;
  }
}

void dtGmshMessagePrinter::registerPrinter()
{
  static dtGmshMessagePrinter printer;

  Msg::SetCallback(&printer);
}

} // namespace dtOO
