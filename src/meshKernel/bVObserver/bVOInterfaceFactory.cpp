#include "bVOInterfaceFactory.h"

#include <logMe/logMe.h>
#include <string>
#include "bVOInterfaceFactory.h"
#include "bVOInterface.h"
#include "bVOSetGrading.h"
#include "bVONameFaces.h"
#include "bVOSetGradingToFaceRule.h"
#include "bVONameRegions.h"
#include "bVORemoveRegions.h"
#include "bVOWriteMSH.h"
#include "bVOReadMSH.h"
#include "bVOTransfiniteFaces.h"
#include "bVOSetNElements.h"
#include "bVOTransfiniteRegions.h"
#include "bVOOrientCellVolumes.h"
#include "bVORecombineRecursiveRegions.h"
#include "bVOMeshRule.h"
#include "bVOPMeshRule.h"
#include "bVOFaceToPatchRule.h"
#include "bVOAnalyticGeometryToFace.h"
#include "bVORecombine.h"
#include "bVOWriteSTL.h"
#include "bVOSetPrescribedMeshSizeAtPoints.h"
#include "bVODumpModel.h"
#include "bVOEvilGodfather.h"
#include "bVOAddInternalEdge.h"
#include "bVOAddFace.h"
#include "bVOSetRotationalPeriodicity.h"
#include "bVOSetTranslationalPeriodicity.h"
#include "bVOTransformMeshPoints.h"
#include "bVOWriteSU2.h"
#include "bVOSetPrescribedFirstElementSize.h"
#include "bVOForceMeshPointCoordinates.h"
#include "bVOAnalyticFunctionToFace.h"
#include "bVOWriteINP.h"

namespace dtOO {
  dt__pH(bVOInterfaceFactory) bVOInterfaceFactory::_instance(NULL);
  
  bVOInterfaceFactory::bVOInterfaceFactory() {
  }

  bVOInterfaceFactory::~bVOInterfaceFactory() {
    _builder.destroy();        
  }

  bVOInterface * bVOInterfaceFactory::create( std::string const str ) {
    dt__forAllRefAuto( instance()->_builder, aBuilder ) {
      //
      // check virtual class name
      //
      if ( aBuilder->virtualClassName() == str ) {
        return aBuilder->create();
      }
            
      //
      // check alias
      //
      dt__forAllRefAuto(aBuilder->factoryAlias(), anAlias) {
        if ( anAlias == str ) return aBuilder->create();
      }
    }

    std::vector< std::string > av;
    dt__forAllRefAuto( instance()->_builder, aBuilder ) {
      av.push_back( aBuilder->virtualClassName() );
      dt__forAllRefAuto(aBuilder->factoryAlias(), anAlias) {
        av.push_back("  -> "+anAlias); 
      }      
    }
    dt__throw(
      create(), 
      << str <<  " could not be created." << std::endl
      << "Implemented builder:" << std::endl
      << logMe::vecToString(av,1) << std::endl
    );              
  }
  
  bVOInterface * bVOInterfaceFactory::create( char const * const str ) {
    return create( std::string(str) );
  }

  bVOInterfaceFactory * bVOInterfaceFactory::instance( void ) {
    if ( !_instance.isNull() ) return _instance.get();
    
    _instance.reset( new bVOInterfaceFactory() );
    
    //
    // add builder
    //
    _instance->_builder.push_back( new bVOSetGrading() );
    _instance->_builder.push_back( new bVONameFaces() );
    _instance->_builder.push_back( new bVONameRegions() );
    _instance->_builder.push_back( new bVORemoveRegions() );
    _instance->_builder.push_back( new bVOWriteMSH() );
    _instance->_builder.push_back( new bVOReadMSH() );
    _instance->_builder.push_back( new bVOTransfiniteFaces() );
    _instance->_builder.push_back( new bVOSetNElements() );
    _instance->_builder.push_back( new bVOTransfiniteRegions() );
    _instance->_builder.push_back( new bVOOrientCellVolumes() );
    _instance->_builder.push_back( new bVORecombineRecursiveRegions() );
    _instance->_builder.push_back( new bVOMeshRule() );
    _instance->_builder.push_back( new bVOPMeshRule() );
    _instance->_builder.push_back( new bVOFaceToPatchRule() );
    _instance->_builder.push_back( new bVOAnalyticGeometryToFace() );
    _instance->_builder.push_back( new bVORecombine() );
    _instance->_builder.push_back( new bVOWriteSTL() );
    _instance->_builder.push_back( new bVOSetPrescribedMeshSizeAtPoints() );
    _instance->_builder.push_back( new bVODumpModel() );
    _instance->_builder.push_back( new bVOEvilGodfather);
    _instance->_builder.push_back( new bVOAddInternalEdge() );
    _instance->_builder.push_back( new bVOAddFace() );
    _instance->_builder.push_back( new bVOSetRotationalPeriodicity() );
    _instance->_builder.push_back( new bVOSetTranslationalPeriodicity() );
    _instance->_builder.push_back( new bVOTransformMeshPoints() );
    _instance->_builder.push_back( new bVOWriteSU2() );
    _instance->_builder.push_back( new bVOSetGradingToFaceRule() );
    _instance->_builder.push_back( new bVOSetPrescribedFirstElementSize() );
    _instance->_builder.push_back( new bVOForceMeshPointCoordinates() );
    _instance->_builder.push_back( new bVOAnalyticFunctionToFace() );
    _instance->_builder.push_back( new bVOWriteINP() );
    return _instance.get();
  }    
}