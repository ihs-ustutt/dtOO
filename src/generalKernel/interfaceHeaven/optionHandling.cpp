#include "optionHandling.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <sstream>
#include <algorithm>

namespace dtOO {  
  optionHandling::optionHandling() {
    std::vector< std::string > groupStrVec;
    std::pair< std::vector< std::string >, dtReal > groupPair;
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
  
  void optionHandling::init( ::QDomElement const * const wElement) {
    ::QDomElement option = qtXmlPrimitive::getChild("option", *wElement);
    while ( !option.isNull() ) {
      optionHandling::setOption(
        qtXmlPrimitive::getAttributeStr("name", option), 
        qtXmlPrimitive::getAttributeStr("value", option)
      );
      option = option.nextSiblingElement("option");
    }
  }

  void optionHandling::jInit( jsonPrimitive const & jE) {
    if ( jE.contains("option") ) {
      dt__forAllRefAuto(
        jE.lookup<std::vector<jsonPrimitive>>("option"), anOpt
      ) {
        this->setOption(
          anOpt.lookup<std::string>("name"),
          anOpt.lookup<std::string>("value")
        );
      }
    }
  }
  
  void optionHandling::init(
    ::QDomElement const & wElement,
    baseContainer const * const bC,      
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG
  ) {
    dt__forAllRefAuto(
      qtXmlPrimitive::getChildVector("option", wElement), anOption
    ) {
      optionHandling::setOption(
        dtXmlParserBase::getAttributeStr("name", anOption), 
        dtXmlParserBase::replaceDependencies(
          dtXmlParserBase::getAttributeStr("value", anOption), bC, cV, aF, aG
        )
      );
    }        
  }
      
  void optionHandling::setOption(
    std::string const name, std::string const value
  ) {
    std::vector< std::string >::iterator it 
    = 
    std::find(_optionName.begin(), _optionName.end(), name);
    
    if (it == _optionName.end()) {
      _optionName.push_back(name);
      _optionValue.push_back(value);
    }
    else {
      _optionValue[
        static_cast< dtInt >(it - _optionName.begin())
      ]
      =
      value;
    }
  }
  
  std::string optionHandling::getOption(
    std::string const name, std::string const val
  ) const {
    for (int ii=0;ii<_optionName.size();ii++) {
      if (_optionName[ii] == name) {
        return _optionValue[ii];
      }
    }
    return val;
  }
  
  std::string optionHandling::getOption(std::string const name) const {
    std::string val = getOption(name, "");
    
    dt__throwIfWithMessage(
      val == "",
      getOption(), 
      << "Option " << dt__eval(name) << " not found."
    );    
    
    
    return val;
  }
  
  bool optionHandling::hasOption(std::string const name) const {
    if (getOption(name, "") == "") {
      return false;
    }
    return true;
  }

  dtReal optionHandling::getOptionFloat(std::string const name) const {
    dtReal argumentFloat;
    std::istringstream( getOption(name) ) >> argumentFloat;
    return argumentFloat;
  }
	
  dtInt optionHandling::getOptionInt(std::string const name) const {
    dtInt argumentInt;
    std::istringstream( getOption(name) ) >> argumentInt;
    return argumentInt;
  }
  
  bool optionHandling::optionTrue(std::string const name) const {
    for (int ii=0;ii<_optionName.size();ii++) {
      if (_optionName[ii] == name) {
        if (_optionValue[ii] == "true" ) {
          return true;
        }
        else if ( _optionValue[ii] == "false" ) {
          return false;
        }
        else {
          dt__warning(
            optionTrue(),
            << "Option " << dt__eval(name) << " is set to " 
            << _optionValue[ii] << "."
          );    
          return false;
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
      if ( 
        stringPrimitive::getStringBetween("[", "]", _optionName[ii]) == name 
      ) {
        std::string optionNameTwin = _optionName[ii];
        stringPrimitive::getStringBetweenAndRemove( 
          "[", "]", &(optionNameTwin) 
        );
        optionGroupElement toAdd;
        while ( stringPrimitive::stringContains(".", optionNameTwin) ) {
          toAdd.first.push_back(
            stringPrimitive::getStringBetweenAndRemove( 
              "", ".", &optionNameTwin 
            )
          );
        }
        toAdd.first.push_back( optionNameTwin );
        toAdd.second = qtXmlBase::muParseString( _optionValue[ii] );
        group.push_back(toAdd);
      }
    }
    return group;
  }  

  optionGroupInt optionHandling::getOptionGroupInt( 
    std::string const name 
  ) const {
    optionGroup group;
    
    for (int ii=0;ii<_optionName.size();ii++) {
      if ( 
        stringPrimitive::getStringBetween("[", "]", _optionName[ii]) == name 
      ) {
        std::string optionNameTwin = _optionName[ii];
        stringPrimitive::getStringBetweenAndRemove( 
          "[", "]", &(optionNameTwin) 
        );
        optionGroupElement toAdd;
        while ( stringPrimitive::stringContains(".", optionNameTwin) ) {
          toAdd.first.push_back(
            stringPrimitive::getStringBetweenAndRemove( 
              "", ".", &(optionNameTwin) 
            )
          );
        }
        toAdd.first.push_back(
          optionNameTwin
        );
        toAdd.second = qtXmlBase::muParseStringInt( _optionValue[ii] );
        group.push_back(toAdd);
      }
    }
    return group;
  }
  
  bool optionHandling::debugTrue( void ) const {
    return 
      staticPropertiesHandler::getInstance()->gDebug()
      ||
      optionTrue("debug");
  }
}
