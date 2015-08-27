#include "intParam.h"

#include <logMe/logMe.h>

namespace dtOO {
  intParam::intParam() {
  }

  intParam::intParam(intParam const & orig) : constValue(orig) {
  }

  intParam::~intParam() {
  }

  intParam * intParam::clone( void ) const {
    return new intParam( *this );
  }  

  void intParam::dump(void) const {
    dt__info( 
      dump(),
      << getLabel() << std::endl
      << dt__eval( getValue() ) 
    );  
  }

  void intParam::writeToElement(
    ::QDomDocument & doc, ::QDomElement & element
  ) {
    ::QDomElement cValElement = qtXmlPrimitive::createElement(doc, "constValue");
    cValElement.setAttribute("label", getLabel().c_str());
    cValElement.setAttribute("name", "intParam");
    cValElement.setAttribute("value", getValue());

    element.appendChild(cValElement);   
  }  
  
}