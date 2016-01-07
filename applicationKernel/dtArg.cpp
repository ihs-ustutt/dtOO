#include "dtArg.h"
#include <iostream>

namespace dtOO {  
  dtArg::dtArg( 
    std::string const & appName, int argC, char const * const argV[] 
  ) : dtArgMap(), _desc("Allowed options") {
    _argC = argC;    
    _argV = argV;
    _desc.add_options()
      ("help,h", "produce help message")
      (
        "log,l", 
        dtPO::value<std::string>()->default_value(
          appName+".log"
        ), 
        "define logfile (optional)"
      )
    ;
  }

  dtArg::~dtArg() {
  }
  
  void dtArg::setMachine( void ) {
    _desc.add_options()
      (
        "xmlIn,i", 
        dtPO::value<std::string>()->required(), 
        "set input xml file (required)"
      )
      (
        "xmlOut,o", 
        dtPO::value<std::string>()->required(), 
        "set output xml file (required)"
      )
    ;
  }
  
  dtArgDesc & dtArg::description( void ) {
    return _desc;
  }
  
  void dtArg::update( void ) {
    try {
      dtPO::store( dtPO::parse_command_line(_argC, _argV, _desc), *this );
      dtPO::notify(*this);    
    }
    catch(dtPO::required_option& e ) {
      std::cout 
        << "Catch dtPO::required_option exception." << std::endl
        << "dtPO::required_option.what() = " << e.what() << std::endl
        << _desc << std::endl;
      exit(0);      
    }

    //
    // show help
    //
    if ( this->count("help") ) {
      std::cout << _desc << std::endl;
      exit(0);
    }    
  }
  
  std::string dtArg::callCommand(void) {
    std::vector< std::string > callArg(_argV, _argV+_argC);
    std::string retStr;
    std::for_each( 
      callArg.begin(), callArg.end(), [&retStr](std::string & aStr) { 
        retStr = retStr + " " + aStr;
      }
    );

    return retStr;    
  }
}
