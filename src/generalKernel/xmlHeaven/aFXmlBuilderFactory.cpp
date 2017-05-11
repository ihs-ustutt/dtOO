#include "aFXmlBuilderFactory.h"

#include <logMe/logMe.h>
#include <string.h>

#include "aFXmlBuilder/analyticFunctionAFXmlBuilder.h"
#include "aFXmlBuilder/analyticFunctionCombinationAFXmlBuilder.h"
#include "aFXmlBuilder/baseContainerAFXmlBuilder.h"
#include "aFXmlBuilder/bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder.h"
#include "aFXmlBuilder/bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder.h"
#include "aFXmlBuilder/bSplineCurve2d_pointConstructOCCAFXmlBuilder.h"
#include "aFXmlBuilder/bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder.h"
#include "aFXmlBuilder/bSplineCurve_pointConstructOCCAFXmlBuilder.h"
#include "aFXmlBuilder/bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder.h"
#include "aFXmlBuilder/bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder.h"
#include "aFXmlBuilder/bSplineSurface_skinConstructOCCAFXmlBuilder.h"
#include "aFXmlBuilder/muParserAFXmlBuilder.h"
#include "aFXmlBuilder/scaOneD_forceBSplineCurve2dAFXmlBuilder.h"
#include "aFXmlBuilder/vec3dThreeD_transVolAFXmlBuilder.h"
#include "aFXmlBuilder/vec3dThreeD_triLinearPointConstructAFXmlBuilder.h"
#include "aFXmlBuilder/vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder.h"
#include "aFXmlBuilder/scaSingleTanhGradingOneDAFXmlBuilder.h"
#include "aFXmlBuilder/scaSingleTanhGradingOneDAFXmlBuilder.h"

namespace dtOO {
  dt__pH(aFXmlBuilderFactory) aFXmlBuilderFactory::_instance(NULL);
  
  aFXmlBuilderFactory::~aFXmlBuilderFactory() {
    _builder.destroy();
  }

  aFXmlBuilder * aFXmlBuilderFactory::create(char const * const str) {
    return create( std::string(str) );
  }

  aFXmlBuilder * aFXmlBuilderFactory::create( std::string const str ) {
    dt__forAllRefAuto( instance()->_builder, aBuilder ) {
      //
      // check virtual class name
      //
      if ( aBuilder->virtualClassName() == str ) {
        return aBuilder->create();
      }
      
      //
      // check virtual class name without postFix
      //
      if ( 
        stringPrimitive::replaceStringInString(
          "AFXmlBuilder", "", aBuilder->virtualClassName() 
        ) 
        == 
        str
      ) {
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

  aFXmlBuilderFactory * aFXmlBuilderFactory::instance( void ) {
    if ( !_instance.isNull() ) return _instance.get();
    
    _instance.reset( new aFXmlBuilderFactory() );
    
    //
    // add builder
    //
    _instance->_builder.push_back( new analyticFunctionAFXmlBuilder() );
    _instance->_builder.push_back( 
      new analyticFunctionCombinationAFXmlBuilder() 
    );
    _instance->_builder.push_back( new baseContainerAFXmlBuilder() );
    _instance->_builder.push_back( 
      new bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve2d_pointConstructOCCAFXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve2d_pointInterpolateConstructOCCAFXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineCurve_pointConstructOCCAFXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder() 
    );
    _instance->_builder.push_back( 
      new bSplineSurface_skinConstructOCCAFXmlBuilder() 
    );
    _instance->_builder.push_back( new muParserAFXmlBuilder() );
    _instance->_builder.push_back( 
      new scaOneD_forceBSplineCurve2dAFXmlBuilder() 
    );
    _instance->_builder.push_back( new vec3dThreeD_transVolAFXmlBuilder() );
    _instance->_builder.push_back( 
      new vec3dThreeD_triLinearPointConstructAFXmlBuilder() 
    );
    _instance->_builder.push_back(
      new bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder()
    );
    _instance->_builder.push_back(
      new vec3dThreeD_multiTriLinearPointConstructAFXmlBuilder()
    );
    _instance->_builder.push_back(
    new scaSingleTanhGradingOneDAFXmlBuilder()
    );
    return _instance.get();
  }
  
  aFXmlBuilderFactory::aFXmlBuilderFactory() {
  }  
}
