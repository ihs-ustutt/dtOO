#include "dtCase.h"
#include <logMe/logMe.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <interfaceHeaven/systemHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

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
    optionHandling::init( &element );
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
    
    _status.clear();
    dt__forAllRefAuto(_directory, aDir) {
      createStatus(aDir);
      std::string statusFile = aDir+"/status";
      if ( systemHandling::fileExists(statusFile) ) {
        std::ifstream in( statusFile.c_str() );
        if (in) {
          int status;
          in >> status;
          _status.push_back(status);
          in.close();
        }
        else _status.push_back(UNEXPECTED);
      }
      else _status.push_back(UNEXPECTED);
    }
  }

  std::vector< std::string > dtCase::allStates( void ) const {
    return _state;
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
    else dt__throwUnexpected(status());
  }

  std::string dtCase::statusStr( std::string const & state ) const {
    return STATUSSTRING[ status(state) ];
  }
  std::string dtCase::getDirectory( std::string const & state ) const {
    std::vector< std::string >::const_iterator it
    =
    std::find(_state.begin(), _state.end(), state);
    
    dt__throwIf(it==_state.end(), getDirectory());
    
    return _directory.at( static_cast<int>(it-_state.begin()) );
  }
}
