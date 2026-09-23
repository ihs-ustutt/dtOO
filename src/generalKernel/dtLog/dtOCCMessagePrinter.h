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

#ifndef __dtOCCMessagePrinter_H__
#define __dtOCCMessagePrinter_H__

#include <Message_Printer.hxx>
#include <TCollection_AsciiString.hxx>

namespace dtOO {

class dtOCCMessagePrinter : public Message_Printer {
public:
  dtOCCMessagePrinter();
  virtual ~dtOCCMessagePrinter();
  virtual void send(
    const TCollection_AsciiString &message, const Message_Gravity gravity
  ) const;
  static void registerPrinter();
};

} // namespace dtOO

#endif // __dtOCCMessagePrinter_H__
