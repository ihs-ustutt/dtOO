#include "sliderFloatParam.h"
#include <logMe/logMe.h>

namespace dtOO {
  sliderFloatParam::sliderFloatParam() {
  }

  sliderFloatParam::sliderFloatParam(sliderFloatParam const & orig) : constValue(orig) {
    _max = orig._max;
    _min = orig._min;
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

  void sliderFloatParam::setRange(float const min, float const max) {
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
  
  float sliderFloatParam::getMax(void) const {
    return _max;
  }

  float sliderFloatParam::getMin(void) const {
    return _min;
  }  
}