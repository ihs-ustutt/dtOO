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

#include "intParam.h"

#include <logMe/logMe.h>

namespace dtOO {
intParam::intParam(intParam const &orig) : constValue(orig) {}

intParam::intParam(std::string const &label, dtInt const &val)
  : constValue(label, val)
{
}

intParam::~intParam() {}

intParam *intParam::clone(void) const { return new intParam(*this); }

void intParam::dump(void) const
{
  dt__info(dump(), << getLabel() << std::endl << dt__eval(getValue()));
}

void intParam::writeToElement(::QDomDocument &doc, ::QDomElement &element)
{
  ::QDomElement cValElement = qtXmlPrimitive::createElement(doc, "constValue");
  cValElement.setAttribute("label", getLabel().c_str());
  cValElement.setAttribute("name", "intParam");
  cValElement.setAttribute("value", getValue());

  element.appendChild(cValElement);
}

} // namespace dtOO
