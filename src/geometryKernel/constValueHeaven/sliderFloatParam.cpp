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

#include "sliderFloatParam.h"
#include <logMe/logMe.h>

namespace dtOO {
  sliderFloatParam::sliderFloatParam(
    sliderFloatParam const & orig
  ) : constValue(orig) {
  }

  sliderFloatParam::sliderFloatParam(
    std::string const & label, dtReal const & val
  ) : constValue(label, val) {
  }

  sliderFloatParam::sliderFloatParam(
    std::string const & label, 
    dtReal const & val, dtReal const & min, dtReal const & max
  ) : constValue(label, val) {
    setRange(min, max);
  }
  
  sliderFloatParam::~sliderFloatParam() {
  }
  
  sliderFloatParam * sliderFloatParam::clone( void ) const {
    return new sliderFloatParam( *this );
  }

  void sliderFloatParam::dump(void) const {
    dt__info( 
      dump(),
      << getLabel() << std::endl
      << dt__eval( getValue() ) << std::endl
      << dt__eval( getMin() ) << std::endl
      << dt__eval( getMax() ) 
    );
  }

  void sliderFloatParam::writeToElement(
   ::QDomDocument & doc, ::QDomElement & element
  ) {
    ::QDomElement cValElement = qtXmlPrimitive::createElement(doc, "constValue");
    cValElement.setAttribute("label", getLabel().c_str());
    cValElement.setAttribute("name", "sliderFloatParam");
    cValElement.setAttribute("value", getValue());
    cValElement.setAttribute("min", getMin());
    cValElement.setAttribute("max", getMax());

    element.appendChild(cValElement);
  }
}
