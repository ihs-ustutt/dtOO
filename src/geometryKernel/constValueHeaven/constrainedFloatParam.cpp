#include "constrainedFloatParam.h"
#include "xmlHeaven/dtXmlParser.h"
#include <logMe/logMe.h>

namespace dtOO {
  constrainedFloatParam::constrainedFloatParam( std::string const & valueStr ) {
    _valueStr = valueStr;
    _cVArr = NULL;
  }

  constrainedFloatParam::constrainedFloatParam(
    constrainedFloatParam const & orig
  ) : constValue(orig) {
    _max = orig._max;
    _min = orig._min;
    _valueStr = orig._valueStr;
  }

  constrainedFloatParam::~constrainedFloatParam() {
  }
  
  constrainedFloatParam * constrainedFloatParam::clone( void ) const {
    return new constrainedFloatParam( *this );
  }

  void constrainedFloatParam::dump(void) const {
    dt__info( 
      dump(),
      << getLabel() << std::endl
      << dt__eval( getValue() ) << std::endl
      << dt__eval( _min ) << std::endl
      << dt__eval( _max )  << std::endl
      << dt__eval( _valueStr ) 
    );
  }

  void constrainedFloatParam::setRange(float const min, float const max) {
    _max = max;
    _min = min;
  }

  void constrainedFloatParam::writeToElement(
   ::QDomDocument & doc, ::QDomElement & element
  ) {
    ::QDomElement cValElement = qtXmlPrimitive::createElement(doc, "constValue");
    cValElement.setAttribute("label", getLabel().c_str());
    cValElement.setAttribute("name", "constrainedFloatParam");
    cValElement.setAttribute("value", _valueStr.c_str() );
    cValElement.setAttribute("min", _min);
    cValElement.setAttribute("max", _max);

    element.appendChild(cValElement);
  }
  
  float constrainedFloatParam::getMax(void) const {
    return _max;
  }

  float constrainedFloatParam::getMin(void) const {
    return _min;
  }  

  float constrainedFloatParam::getValue(void) const {
    return dtXmlParser::constReference().muParseString(
      dtXmlParser::constReference().replaceDependencies( _valueStr, _cVArr)
    );
  }

  void constrainedFloatParam::setValue(float const toSet) {
    dt__warning(
      setValue(), 
      << "Try to set a constrainedFloatParam > " 
      << getLabel() << " <. Not possible. Set to constraint."
    );
    constValue::setValue(
      std::numeric_limits< float >::infinity()
    );
  }  
  
  bool constrainedFloatParam::loadable( void ) const {
    return false;  
  }
  
  void constrainedFloatParam::resolveConstraint( 
    cVPtrVec const * const cVArr 
  ) {
    _cVArr = cVArr;
  }
}