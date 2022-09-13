#include "sliderFloatParam.h"
#include <logMe/logMe.h>

namespace dtOO {
  sliderFloatParam::sliderFloatParam() {
  }

  sliderFloatParam::sliderFloatParam(
    sliderFloatParam const & orig
  ) : constValue(orig) {
    _max = orig._max;
    _min = orig._min;
  }

  sliderFloatParam::sliderFloatParam(
    std::string const & label, 
    dtReal const & val, dtReal const & min, dtReal const & max
  ) : constValue(label, val) {
    _min = min;
    _max = max;
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
      << dt__eval( _min ) << std::endl
      << dt__eval( _max ) 
    );
  }

  void sliderFloatParam::setRange(dtReal const min, dtReal const max) {
    _max = max;
    _min = min;
  }

  void sliderFloatParam::writeToElement(
   ::QDomDocument & doc, ::QDomElement & element
  ) {
    ::QDomElement cValElement = qtXmlPrimitive::createElement(doc, "constValue");
    cValElement.setAttribute("label", getLabel().c_str());
    cValElement.setAttribute("name", "sliderFloatParam");
    cValElement.setAttribute("value", getValue());
    cValElement.setAttribute("min", _min);
    cValElement.setAttribute("max", _max);

    element.appendChild(cValElement);
  }
  
  dtReal sliderFloatParam::getMax(void) const {
    return _max;
  }

  dtReal sliderFloatParam::getMin(void) const {
    return _min;
  }  
}