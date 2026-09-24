/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >

    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in
    the dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "dtOCCMessagePrinter.h"

#include <Message.hxx>
#include <TCollection_ExtendedString.hxx>
#include <dtLog/dtLog.h>

namespace dtOO {

dtOCCMessagePrinter::dtOCCMessagePrinter() : Message_Printer() {}

dtOCCMessagePrinter::~dtOCCMessagePrinter() {}

void dtOCCMessagePrinter::send(
  const TCollection_AsciiString &message, const Message_Gravity gravity
) const
{
  const std::string messageString(message.ToCString());

  switch (gravity)
  {
  case Message_Info:
    dtLog__info << messageString;
    break;

  case Message_Warning:
    dtLog__warning << messageString;
    break;

  case Message_Alarm:
    dtLog__error << messageString;
    break;

  case Message_Fail:
    dtLog__fatal << messageString;
    break;

  default:
    dtLog__debug << messageString;
    break;
  }
}

void dtOCCMessagePrinter::registerPrinter()
{
  Handle(dtOCCMessagePrinter) printer = new dtOCCMessagePrinter();
  Message::DefaultMessenger()->AddPrinter(printer);
}

} // namespace dtOO
