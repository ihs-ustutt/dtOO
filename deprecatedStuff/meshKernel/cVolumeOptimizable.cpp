#include "cVolumeOptimizable.h"
#include "interfaceHeaven/systemHandling.h"

#include <logMe/logMe.h>

namespace dtOO {
  DTCLASSLOGMETHODI(cVolumeOptimizable,
          << "No member attributes");
  
  cVolumeOptimizable::cVolumeOptimizable() : cVolume() {
  }

  cVolumeOptimizable::cVolumeOptimizable(const cVolumeOptimizable& orig) : cVolume(orig) {
  }

  cVolumeOptimizable::~cVolumeOptimizable() {
  }

  void cVolumeOptimizable::init(
         QDomElement const & element,
         vectorHandling< constValue * > const * const cValP,
         vectorHandling< scaFunction * > const * const sFunP,
         vectorHandling< analyticGeometry * > const * const depAGeoP 
       ) {
    //
    // init cVolume
    //
    cVolume::init(element, cValP, sFunP, depAGeoP);
    
    //
    // read failed and worst design
    //
    std::fstream failedFile;
    std::fstream worstFile;
    std::string str;
    str = _caseDirectory+"/failed.txt";
    failedFile.open( str.c_str(), std::fstream::in );
    str = _caseDirectory+"/worst.txt";
    worstFile.open( str.c_str(), std::fstream::in );    
    
    //
    // current result value
    //
    failedFile >> _meshFailed;
    //
    // worst result value
    //
    worstFile >> _meshWorst;

    failedFile.close();           
    worstFile.close();           
  }
    
  float cVolumeOptimizable::characterizeMe( void ) {
    
    readResultCheckMesh();
    
    float val = _meshQuality;
 
    DTINFOWF(characterizeMe(),
            << DTLOGEVAL(val) );
    return val;
  }

  float cVolumeOptimizable::characterizeFailedDesign( void ) {
    return _meshFailed;
  }
  
//  void cVolumeOptimizable::doCycle( void ) {
//    //
//    // recursive call
//    //
//    optimizationCyclingDecorator::doCycle();
//
//    systemHandling systemH;
//    int ii = 0;
//    while(true) {
//      _caseDirectory = _caseDirectory+intToString(ii);
//      //
//      // try to create directory
//      //
//      if ( !systemH.createDirectory(_caseDirectory) ) {
//        ii++;
//      }
//      else {       
//        //
//        // store temp directory in eGeneral
//        //
//        eGeneral().bringDirectoryToGrave(_caseDirectory);
//        break;
//      }
//    }
//
//    //
//    // my work
//    //
//    cVolume::writeGrid();
//
//    //
//    // read mesh quality
//    //
//    readResultCheckMesh();
//
//    //
//    // delete my files
//    //
//    std::string cmd = "rm -rf " + _caseDirectory;
//    systemH.command( cmd );
//
//    
//    if ( cVolume::parameterCheck() ) {
//      DTINFOWF(writeGrid(),
//              << DTLOGEVAL(_meshQuality) );
//    }
//    else {
//      _meshQuality = _meshWorst;
//    }
//  }

  void cVolumeOptimizable::readResultCheckMesh( void ) {
    //
    // create blockMeshDict file
    //
    std::fstream resultFile;
    std::string resultFileStr = _caseDirectory+"/result.txt";
    resultFile.open( resultFileStr.c_str(), std::fstream::in );
    
    //
    // current result value
    //
    resultFile >> _meshQuality;

    resultFile.close();       
  }  
}
