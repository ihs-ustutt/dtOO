#include "dtTransformerFactory.h"
#include <logMe/logMe.h>
#include "dtTransformer.h"
#include "dtTransformerKernel.h"
#include "dtTransformerDriver.h"
#include "doNothing.h"
#include "thicknessIncreasing.h"
#include "biThicknessIncreasing.h"
#include "offset.h"
#include "translate.h"
#include "rotate.h"
#include "pickMap3dTo3dRangePercent.h"
#include "pickMap2dTo3dRangePercent.h"
#include "makePolynomial.h"
#include "predefinedExtension.h"
#include "closeGaps.h"
#include "pickLengthRange.h"
#include "pickLengthPercentRange.h"
#include "addConstCoordinate.h"
#include "uVw_phirMs.h"
#include "uVw_skewPhirMs.h"
#include "uVw_phiMs.h"
#include "uVw_deltaMs.h"
#include "averagePoints.h"
#include "projectOnSurface.h"
#include "reparamInSurface.h"
#include "pickVec3dTwoDRangePercent.h"
#include "analyticAddNormal.h"
#include "discreteAddNormal.h"
#include "approxInSurface.h"
#include "normalOffsetInSurface.h"
#include "closeGapsArithmetic.h"
#include "scale.h"
#include "xYz_rPhiZ.h"
#include "pickMap2dTo3dRectanglePercent.h"
#include "reverse.h"
#include "pickMap1dTo3dLinePercent.h"
#include "applyVec3dThreeD.h"

namespace dtOO {
  dt__pH(dtTransformerFactory) dtTransformerFactory::_instance(NULL);
  
  dtTransformerFactory::dtTransformerFactory() {
  }

  dtTransformerFactory::~dtTransformerFactory() {
    _transformer.destroy();      
  }

  dtTransformer* dtTransformerFactory::create( std::string const str ) {
    dt__forAllRefAuto( instance()->_transformer, aTransformer ) {
      //
      // check virtual class name
      //
      if ( aTransformer->virtualClassName() == str ) {
        return aTransformer->create();
      }
            
      //
      // check alias
      //
      dt__forAllRefAuto(aTransformer->factoryAlias(), anAlias) {
        if ( anAlias == str ) return aTransformer->create();
      }
    }

    std::vector< std::string > av;
    dt__forAllRefAuto( instance()->_transformer, aTrans ) {
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

  dtTransformer* dtTransformerFactory::create( char const * const str ) {
    return create( std::string(str) );
  }
  
  dtTransformer * dtTransformerFactory::createFromPlugin(
    std::string const & str, 
    std::string const & pluginName, 
    std::string const & pluginDriver
  ) {
    dt__info( createFromPlugin(), << "creating " << str <<  "..." );
  
    //
    // init kernel
    //
    dtTransformerKernel * kernel = new dtTransformerKernel();
    kernel->add_server(dtTransformer::server_name(), dtTransformer::version);  

    //
    // output
    //
    dt__info(
      createFromPlugin(), 
      << dt__eval(pluginName) << std::endl
      << dt__eval(pluginDriver)
    );    
    
    //
    // load shared library
    //
    dt__throwIf( 
      !kernel->load_plugin(pluginName), << "Loading plugin failed."
    );

    //
    // create plugin using plugin driver
    //
    dtTransformer *  ret 
    = 
    kernel->get_driver< dtTransformerDriver >(
      dtTransformer::server_name(), pluginDriver
    )->create();

    //
    // store kernel in plugin
    //
    ret->setKernel(kernel);
    
    return ret;
  }  
  
  dtTransformerFactory * dtTransformerFactory::instance( void ) {
    if ( !_instance.isNull() ) return _instance.get();
    
    _instance.reset( new dtTransformerFactory() );
    
    //
    // add builder
    //
    _instance->_transformer.push_back( new doNothing() );
    _instance->_transformer.push_back( new thicknessIncreasing() );
    _instance->_transformer.push_back( new biThicknessIncreasing() );
    _instance->_transformer.push_back( new offset() );
    _instance->_transformer.push_back( new translate() );
    _instance->_transformer.push_back( new rotate() );
    _instance->_transformer.push_back( new pickMap3dTo3dRangePercent() );
    _instance->_transformer.push_back( new pickMap2dTo3dRangePercent() );
    _instance->_transformer.push_back( new makePolynomial() );
    _instance->_transformer.push_back( new predefinedExtension() );
    _instance->_transformer.push_back( new closeGaps() );
    _instance->_transformer.push_back( new pickLengthRange() );
    _instance->_transformer.push_back( new pickLengthPercentRange() );
    _instance->_transformer.push_back( new addConstCoordinate() );
    _instance->_transformer.push_back( new uVw_phirMs() );
    _instance->_transformer.push_back( new uVw_phiMs() );
    _instance->_transformer.push_back( new uVw_deltaMs() );
    _instance->_transformer.push_back( new averagePoints() );
    _instance->_transformer.push_back( new projectOnSurface() );
    _instance->_transformer.push_back( new reparamInSurface() );
    _instance->_transformer.push_back( new pickVec3dTwoDRangePercent() );
    _instance->_transformer.push_back( new analyticAddNormal() );
    _instance->_transformer.push_back( new discreteAddNormal() );
    _instance->_transformer.push_back( new approxInSurface() );
    _instance->_transformer.push_back( new normalOffsetInSurface() );
    _instance->_transformer.push_back( new closeGapsArithmetic() );
    _instance->_transformer.push_back( new scale() );
    _instance->_transformer.push_back( new xYz_rPhiZ() );
    _instance->_transformer.push_back( new uVw_skewPhirMs() );
    _instance->_transformer.push_back( new pickMap2dTo3dRectanglePercent() );
    _instance->_transformer.push_back( new reverse() );
    _instance->_transformer.push_back( new pickMap1dTo3dLinePercent() );
    _instance->_transformer.push_back( new applyVec3dThreeD() );
    
    return _instance.get();
  }      
}