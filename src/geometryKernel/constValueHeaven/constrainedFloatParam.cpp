#include "constrainedFloatParam.h"
#include "xmlHeaven/dtXmlParser.h"
#include <logMe/logMe.h>

namespace dtOO {
  constrainedFloatParam::constrainedFloatParam(
    std::string const & label, std::string const & valueStr
  ) : constValue(label, 0.0) {
    _valueStr = valueStr;
    _cVArr = NULL;
  }


  constrainedFloatParam::constrainedFloatParam(
    std::string const & label, 
    std::string const & valueStr, dtReal const & min, dtReal const & max
  ) : constValue(label, 0.0) {
    _valueStr = valueStr;
    setRange(min, max);
    _cVArr = NULL;
  }

  constrainedFloatParam::constrainedFloatParam(
    constrainedFloatParam const & orig
  ) : constValue(orig) {
    _valueStr = orig._valueStr;
    _cVArr = orig._cVArr;
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
      << dt__eval( getMin() ) << std::endl
      << dt__eval( getMax() )  << std::endl
      << dt__eval( _valueStr ) 
    );
  }

  void constrainedFloatParam::writeToElement(
   ::QDomDocument & doc, ::QDomElement & element
  ) {
    ::QDomElement cValElement = qtXmlPrimitive::createElement(doc, "constValue");
    cValElement.setAttribute("label", getLabel().c_str());
    cValElement.setAttribute("name", "constrainedFloatParam");
    cValElement.setAttribute("value", _valueStr.c_str() );
    cValElement.setAttribute("min", getMin());
    cValElement.setAttribute("max", getMax());

    element.appendChild(cValElement);
  }
  
  dtReal constrainedFloatParam::getValue(void) const {
    if (_cVArr) {
      return dtXmlParser::constReference().muParseString(
        dtXmlParser::constReference().replaceDependencies( _valueStr, _cVArr)
      );
    }
    dt__warning(
      getValue(), 
      << "Return 0.0 : Cannot determine correct value, _cVArr = " << _cVArr
    );
    return 0.0;
  }

  std::string constrainedFloatParam::getValueStr(void) const {
    return _valueStr; 
  }

  void constrainedFloatParam::setValue(dtReal const toSet) {
    dt__warning(
      setValue(), 
      << "Try to set a constrainedFloatParam > " 
      << getLabel() << " <. Not possible. Set to constraint."
    );
    constValue::setValue(
      std::numeric_limits< dtReal >::infinity()
    );
  }  
  
  bool constrainedFloatParam::loadable( void ) const {
    return false;  
  }
  
  void constrainedFloatParam::resolveConstraint( 
    lvH_constValue const * const cVArr 
  ) {
    _cVArr = cVArr;
  }
}
