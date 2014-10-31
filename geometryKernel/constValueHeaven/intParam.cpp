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
    DTINFOWF( dump(),
            << getLabel() << LOGDEL
            << DTLOGEVAL( getValue() ) );  
  }

  void intParam::writeToElement(QDomDocument & doc, QDomElement & element) {
    //
    // const Value
    //
    QDomElement cValElement = createElement(doc, "constValue");
    cValElement.setAttribute("label", getLabel().c_str());

    //
    // builder
    //
    QDomElement builderElement = createElement(doc, "builder");
    builderElement.setAttribute("name", "intParam");
    
    //
    //float
    //
    QDomElement intElement = createElement(doc, "int");
    intElement.setAttribute("value", getValue());

    builderElement.appendChild(intElement);    
    cValElement.appendChild(builderElement);
    element.appendChild(cValElement);    
  }  
  
}