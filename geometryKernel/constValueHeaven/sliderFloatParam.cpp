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
    DTINFOWF( dump(),
            << getLabel() << LOGDEL
            << DTLOGEVAL( getValue() ) << LOGDEL
            << DTLOGEVAL( _min ) << LOGDEL
            << DTLOGEVAL( _max ) );
  }

  void sliderFloatParam::setRange(float const min, float const max) {
    _max = max;
    _min = min;
  }

  void sliderFloatParam::writeToElement(QDomDocument & doc, QDomElement & element) {
    //
    // const Value
    //
    QDomElement cValElement = createElement(doc, "constValue");
    cValElement.setAttribute("label", getLabel().c_str());

    //
    // builder
    //
    QDomElement builderElement = createElement(doc, "builder");
    builderElement.setAttribute("name", "sliderFloatParam");
    
    //
    //float
    //
    QDomElement floatElement = createElement(doc, "float");
    floatElement.setAttribute("value", getValue());
    floatElement.setAttribute("min", _min);
    floatElement.setAttribute("max", _max);

    builderElement.appendChild(floatElement);    
    cValElement.appendChild(builderElement);
    element.appendChild(cValElement);
  }
  
  float sliderFloatParam::getMax(void) const {
    return _max;
  }

  float sliderFloatParam::getMin(void) const {
    return _min;
  }  
}