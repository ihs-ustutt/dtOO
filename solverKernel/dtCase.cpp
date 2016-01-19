#include "dtCase.h"

#include <logMe/logMe.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <resultValueHeaven/resultValue.h>
#include <resultValueHeaven/floatValue.h>
#include <interfaceHeaven/systemHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include <interfaceHeaven/stringPrimitive.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace dtOO {
  int const dtCase::SUCCESS = 0;
  int const dtCase::RUNNING = 1;
  int const dtCase::ERROR = 2;
  int const dtCase::UNEXPECTED = 3;
  std::string const dtCase::STATUSSTRING[] 
  = 
  { "SUCCESS", "RUNNING", "ERROR", "UNEXPECTED"};
  
	dtCase::dtCase() {
	}

	dtCase::~dtCase() {
	}
	
  void dtCase::init(
	  ::QDomElement const & element,
		baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtCase * > const * const dC
	) {  
    //
    // set label of dtPlugin
    //
    labelHandling::setLabel( 
		  dtXmlParserBase::getAttributeStr("label", element ) 
		);
    
    //
    // set options
    //
    optionHandling::init( element, cV );
  }	

  void dtCase::update( void ) {
    std::vector< fpath > dL
    =
    systemHandling::directoryList(
      staticPropertiesHandler::getInstance()->getOption("workingDirectory")  
    );
    
    _state.clear();
    _directory.clear();    
    dt__forAllRefAuto(dL, aDL) {
      if ( 
        stringPrimitive::stringContains( 
          getLabel()+"_", aDL.filename().string() 
        ) 
      ) {
        _state.push_back(
          stringPrimitive::replaceStringInString( 
            getLabel()+"_", "", aDL.filename().string()
          )
        ); 
        _directory.push_back( aDL.filename().string() );
      }
    }
    
    _status.resize(_directory.size(), UNEXPECTED);
        dt__forAllIndex(_directory, ii) {
      std::string statusFile = _directory[ii]+"/status";
      
      if ( systemHandling::fileExists(statusFile) ) {
        std::ifstream in( statusFile.c_str() );
        if (in) {
          in >> _status[ii];
          in.close();
        }
      }
    }
  }

  std::vector< std::string > dtCase::allStates( void ) const {
    return _state;
  }

  std::string dtCase::createDirectory( std::string const & state ) const {
    std::string wDir 
    = 
    staticPropertiesHandler::getInstance()->getOption("workingDirectory")
    +
    "/"
    +
    getLabel()+"_"+state;
    
    if ( !systemHandling::directoryExists(wDir) ) {
      systemHandling::createDirectory(wDir);
    }
    
    dt__info( createDirectory(), << dt__eval(wDir) );    

    return wDir;
  }  
//  std::vector< std::string > dtCase::finishedStates( void ) const {
//    std::vector< std::string > running = runningStates();
//    std::vector< std::string > finished = finishedStates();
//    
//    dt__forAllRefAuto(finished, aFinished) {
//      running.push_back( aFinished );
//    }
//    
//    return running;
//  }
  
  bool dtCase::inPipeline( std::string const & state) const {
    std::vector< std::string >::const_iterator it
    =
    std::find(_state.begin(), _state.end(), state);
    if (
      std::find(_state.begin(), _state.end(), state) != _state.end()
    ) return true;
    else return false;
  }
  
  int dtCase::status( std::string const & state ) const {
    std::vector< std::string >::const_iterator it
    =
    std::find(_state.begin(), _state.end(), state);
    if (
      std::find(_state.begin(), _state.end(), state) != _state.end()
    ) {
      return _status.at( static_cast<int>(it-_state.begin()) );
    }
    else return UNEXPECTED;//dt__throwUnexpected(status());
  }

  std::string dtCase::statusStr( std::string const & state ) const {
    return STATUSSTRING[ status(state) ];
  }
  
  std::string dtCase::getDirectory( std::string const & state ) const {
    std::vector< std::string >::const_iterator it
    =
    std::find(_state.begin(), _state.end(), state);
    
    //dt__throwIf(it==_state.end(), getDirectory());
    //
    // create if directory not listed and does not exist
    //
    if( it != _state.end() ) {
      return _directory.at( static_cast<int>(it-_state.begin()) );
    }
    else {
      return createDirectory(state);
    }
  }

  vectorHandling< resultValue * > dtCase::result( 
    std::string const & state 
  ) const {
    vectorHandling< resultValue * > ret(0);
    
    if ( status(state) == SUCCESS ) {
      std::ifstream in( 
        (getDirectory(state)+"/resultValue").c_str() 
      );
      if (in) {
        //
        // read file
        //
        std::string line;
        while (getline(in, line)) {
          boost::algorithm::trim_all(line);
          std::vector< std::string > parts;
          boost::split(
            parts, line, boost::is_any_of(","), boost::token_compress_on
          );

          dt__throwIf(parts.size()!=3, result());
          //
          // add new resultValue
          //
          if (parts[0] == "floatValue") {
            ret.push_back( 
              new floatValue( 
                parts[1], stringPrimitive::stringToFloat(parts[2])
              ) 
            );
          }
          else dt__throwUnexpected(result());
        }
      }
      in.close();    
    }
    
    return ret; 
 }
}
