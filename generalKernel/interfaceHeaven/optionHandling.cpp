#include "optionHandling.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <dtXmlParserDecorator/qtXmlPrimitive.h>
#include <dtXmlParserDecorator/qtXmlBase.h>
#include <sstream>

namespace dtOO {  
  optionHandling::optionHandling() {
    std::vector< std::string > groupStrVec;
    std::pair< std::vector< std::string >, float > groupPair;
    groupPair.first = groupStrVec;
    groupPair.second = 0.;
  }

  optionHandling::optionHandling(const optionHandling& orig) {
    for (int ii=0;ii<orig._optionName.size();ii++) {
      _optionName.push_back(orig._optionName[ii]);
      _optionValue.push_back(orig._optionValue[ii]);
    }
  }

  optionHandling::~optionHandling() {
    _optionName.clear();
    _optionValue.clear();
  }
  
  void optionHandling::init( QDomElement const * const wElement) {
    _optionName.clear();
    _optionValue.clear();

    QDomElement option = qtXmlPrimitive::getChild("option", *wElement);
    while ( !option.isNull() ) {
      std::string optionName = qtXmlPrimitive::getAttributeStr("name", option);
      std::string optionValue = qtXmlPrimitive::getAttributeStr("value", option);
      optionHandling::setOption(optionName, optionValue);
      option = option.nextSiblingElement("option");
    }
  }
  
  void optionHandling::setOption(std::string const name, std::string const value) {
    _optionName.push_back(name);
    _optionValue.push_back(value);
  }
  
  std::string optionHandling::getOption(std::string const name, std::string const val) const {
    for (int ii=0;ii<_optionName.size();ii++) {
      if (_optionName[ii] == name) {
        return _optionValue[ii];
      }
    }
    return val;
  }
  
  std::string optionHandling::getOption(std::string const name) const {
    std::string val = getOption(name, "");
    if (  val != "") {
      return val;
    }
    else {
      dt__throw(getOption(), << "Option " << dt__eval(name) << " not found.");
    }
  }
  
  bool optionHandling::hasOption(std::string const name) const {
    if (getOption(name, "") == "") {
      return false;
    }
    return true;
  }

  float optionHandling::getOptionFloat(std::string const name) const {
    float argumentFloat;
    std::istringstream( getOption(name) ) >> argumentFloat;
    return argumentFloat;
  }

  float optionHandling::getOptionFloat(
	  std::string const name,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF					
	) const {
     return qtXmlBase::muParseString( 
      qtXmlBase::replaceUsedFunctions(getOption(name), cV, aF)
    );
  }
	
  int optionHandling::getOptionInt(std::string const name) const {
    int argumentInt;
    std::istringstream( getOption(name) ) >> argumentInt;
    return argumentInt;
  }
  
  int optionHandling::getOptionInt(
	  std::string const name,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF					
	) const {
     return qtXmlBase::muParseStringInt( 
      qtXmlBase::replaceUsedFunctions(getOption(name), cV, aF)
    );
  }
	
  bool optionHandling::optionTrue(std::string const name) const {
    for (int ii=0;ii<_optionName.size();ii++) {
      if (_optionName[ii] == name) {
        if (_optionValue[ii] == "true" ) {
          return true;
        }
        else if ( _optionValue[ii] == "false" ) {
          break;
        }
        else {
          dt__warning(optionTrue(),
            << "Option " << dt__eval(name) << " is set to " 
            << _optionValue[ii] << ".");    
          break;
        }
      }
    }
    dt__warning(
			optionTrue(),
      << "Option " << dt__eval(name) << " not found. Set to false."
		);    		
    
		return false;
  }

  optionGroup optionHandling::getOptionGroup( std::string const name ) const {
    optionGroup group;
    
    for (int ii=0;ii<_optionName.size();ii++) {
      if ( stringPrimitive::getStringBetween("[", "]", _optionName[ii]) == name ) {
        std::string optionNameTwin = _optionName[ii];
        stringPrimitive::getStringBetweenAndRemove( "[", "]", &(optionNameTwin) );
        optionGroupElement toAdd;
        while ( stringPrimitive::stringContains(".", optionNameTwin) ) {
          toAdd.first.push_back(
            stringPrimitive::getStringBetweenAndRemove( "", ".", &(optionNameTwin) )
          );
        }
        toAdd.first.push_back(
          optionNameTwin
        );
        toAdd.second = stringPrimitive::stringToFloat( _optionValue[ii] );
        group.push_back(toAdd);
      }
    }
    return group;
  }  

  optionGroupInt optionHandling::getOptionGroupInt( std::string const name ) const {
    optionGroup group;
    
    for (int ii=0;ii<_optionName.size();ii++) {
      if ( stringPrimitive::getStringBetween("[", "]", _optionName[ii]) == name ) {
        std::string optionNameTwin = _optionName[ii];
        stringPrimitive::getStringBetweenAndRemove( "[", "]", &(optionNameTwin) );
        optionGroupElement toAdd;
        while ( stringPrimitive::stringContains(".", optionNameTwin) ) {
          toAdd.first.push_back(
            stringPrimitive::getStringBetweenAndRemove( "", ".", &(optionNameTwin) )
          );
        }
        toAdd.first.push_back(
          optionNameTwin
        );
        toAdd.second = stringPrimitive::stringToInt( _optionValue[ii] );
        group.push_back(toAdd);
      }
    }
    return group;
  }  	
}
