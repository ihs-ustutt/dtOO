#include "characterizable.h"

#include <progHelper.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <interfaceHeaven/systemHandling.h>
#include <limits>

namespace dtOO {
  characterizable::characterizable() {
  }

  characterizable::characterizable(const characterizable& orig) {
  }

  characterizable::~characterizable() {
  }
  
  std::string const characterizable::_canReadFileName = std::string("canRead");
  std::string const characterizable::_resValueFileName = std::string("resValue.txt");
  std::string const characterizable::_failedValueFileName = std::string("failedValue.txt");
    
  float characterizable::characterizeMeFailed( void ) const {
    return - std::numeric_limits<float>::max();
  }
    
  void characterizable::characterize( labelHandling const * const lH ) const {
    std::ofstream optiResFile;
    optiResFile.open( _resValueFileName.c_str(), std::ofstream::out | std::ofstream::app );
    optiResFile << std::endl;
    optiResFile << lH->getLabel() << ", " << characterizeMe();  
    optiResFile.close();

    optiResFile.open( _failedValueFileName.c_str(), std::ofstream::out | std::ofstream::app );
    optiResFile << std::endl;
    optiResFile << lH->getLabel() << ", " << characterizeMeFailed();  
    optiResFile.close();
  }
  
  resValueMap characterizable::getResValuesFromFile( void ) const {
    resValueMap ret;
    std::ifstream in;

    in.open( _resValueFileName.c_str(), std::ofstream::in );
    
    if ( !in.good() ) {
      dt__THROW(getResValuesFromFile(),
              << dt__eval(_resValueFileName) << " could not be opened." << std::endl
              << dt__eval(in.good()) );
    }
    
//    // discard header
    std::string line;
//    getline(in, line);

    while ( getline(in, line) ) {
      if (line != "") {
        std::stringstream sep(line);
        std::string field[2];
        getline(sep, field[0], ',');
        getline(sep, field[1], ',');
        ret.insert(         
          std::pair<std::string, float>(
            field[0], 
            stringPrimitive().stringToFloat(field[1])
          ) 
        );
      }
    }
    
        
    in.close();
        
    return ret;
  }  

  resValueMap characterizable::getFailedValuesFromFile( void ) const {
    resValueMap ret;
    std::ifstream in;

    in.open( _failedValueFileName.c_str(), std::ofstream::in );
    
    if ( !in.good() ) {
      dt__THROW(getResValuesFromFile(),
              << dt__eval(_failedValueFileName) << " could not be opened." << std::endl
              << dt__eval(in.good()) );
    }
    
//    // discard header
    std::string line;
//    getline(in, line);

    while ( getline(in, line) ) {
      if (line != "") {
        std::stringstream sep(line);
        std::string field[2];
        getline(sep, field[0], ',');
        getline(sep, field[1], ',');
        ret.insert(         
          std::pair<std::string, float>(
            field[0], 
            stringPrimitive().stringToFloat(field[1])
          ) 
        );
      }
    }
        
    in.close();
        
    return ret;
  }    
  
  void characterizable::sendCanRead( void ) const {
    std::ofstream canRead;
    canRead.open(_canReadFileName.c_str(), std::ofstream::out | std::ofstream::trunc);
    canRead.close();    
  }
  
  bool characterizable::canRead( void ) const {
    return systemHandling().fileExists( _canReadFileName );
  }
  
  void characterizable::removeFiles( void ) const {
    systemHandling().deleteFile(_canReadFileName);
    systemHandling().deleteFile(_resValueFileName);
    systemHandling().deleteFile(_failedValueFileName);
  }
}
