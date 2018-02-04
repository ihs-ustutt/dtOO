#include "boundedVolumeFactory.h"

#include <logMe/logMe.h>
#include "boundedVolume.h"
#include "map2dTo3dTriangulated.h"
#include "map3dTo3dWithInternalTurboGrid.h"
#include "readMOABMesh.h"
#include "map3dTo3dWithInternalGmsh.h"
#include "map3dTo3dWithInternalBlockGmsh.h"
#include "map3dTo3dGmsh.h"
#include "combineGmsh.h"

namespace dtOO {
  dt__pH(boundedVolumeFactory) boundedVolumeFactory::_instance(NULL);
  
  boundedVolumeFactory::boundedVolumeFactory() {
  }

  boundedVolumeFactory::~boundedVolumeFactory() {
    _boundedVolume.destroy();
  }

  boundedVolume * boundedVolumeFactory::create(char const * const str) {
    return create( std::string(str) );
  }

  boundedVolume * boundedVolumeFactory::create( std::string const str ) {
    dt__forAllRefAuto( instance()->_boundedVolume, aBV ) {
      //
      // check virtual class name
      //
      if ( aBV->virtualClassName() == str ) {
        return aBV->create();
      }
            
      //
      // check alias
      //
      dt__forAllRefAuto(aBV->factoryAlias(), anAlias) {
        if ( anAlias == str ) return aBV->create();
      }
    }

    std::vector< std::string > av;
    dt__forAllRefAuto( instance()->_boundedVolume, aTrans ) {
      av.push_back( aTrans->virtualClassName() );
      dt__forAllRefAuto(aTrans->factoryAlias(), anAlias) {
        av.push_back("  -> "+anAlias); 
      }      
    }
    dt__throw(
      create(), 
      << str <<  " could not be created." << std::endl
      << "Implemented transformer:" << std::endl
      << logMe::vecToString(av,1) << std::endl
    );   
  }
  
  boundedVolumeFactory * boundedVolumeFactory::instance( void ) {
    if ( !_instance.isNull() ) return _instance.get();
    
    _instance.reset( new boundedVolumeFactory() );
    
    //
    // add builder
    //
    _instance->_boundedVolume.push_back( new map2dTo3dTriangulated() );
    _instance->_boundedVolume.push_back( new map3dTo3dWithInternalTurboGrid() );
    _instance->_boundedVolume.push_back( new readMOABMesh() );
    _instance->_boundedVolume.push_back( new map3dTo3dWithInternalGmsh() );
    _instance->_boundedVolume.push_back( new map3dTo3dWithInternalBlockGmsh() );
    _instance->_boundedVolume.push_back( new map3dTo3dGmsh() );
    _instance->_boundedVolume.push_back( new combineGmsh() );
    
    return _instance.get();
  }        
}