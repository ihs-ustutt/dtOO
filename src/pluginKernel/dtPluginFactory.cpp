#include "dtPluginFactory.h"

#include <logMe/logMe.h>
#include "dtPlugin.h"
#include "writeStep.h"
#include "constValueAssingRule.h"
#include "analyticFunctionToCSV.h"
#include "volVectorFieldVersusXYZ.h"
#include "volScalarFieldVersusL.h"
#include "pOnBlade.h"
#include "UcylInChannel.h"
#include "uRelInChannel.h"
#include "meshQuality.h"
#include "volScalarInChannelFieldRange.h"
#include "volVectorPatchFieldRange.h"
#include "volScalarPatchFieldRange.h"
#include "dtPluginDriver.h"
#include "dtPluginKernel.h"
#include "volVectorInChannelFieldRange.h"
#include "volVectorFieldRange.h"

namespace dtOO {
  dt__pH(dtPluginFactory) dtPluginFactory::_instance(NULL);

  dtPluginFactory::~dtPluginFactory() {
    _builder.destroy();
  }

  dtPlugin * dtPluginFactory::create( std::string const str ) {
    dt__info( create(), << "str = " << str);
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
  
  
  dtPlugin * dtPluginFactory::createFromPlugin(
    std::string const & str, 
    std::string const & pluginName, 
    std::string const & pluginDriver
  ) {
    dt__info( createFromPlugin(), << "creating " << str <<  "..." );
  
    //
    // init kernel
    //
    dtPluginKernel * kernel = new dtPluginKernel();
    kernel->add_server(dtPlugin::server_name(), dtPlugin::version);  

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
    dtPlugin *  ret 
    = 
    kernel->get_driver< dtPluginDriver >(
      dtPlugin::server_name(), pluginDriver
    )->create();

    //
    // store kernel in plugin
    //
    ret->setKernel(kernel);
    
    return ret;
  }  

  dtPluginFactory * dtPluginFactory::instance( void ) {
    if ( !_instance.isNull() ) return _instance.get();
    
    _instance.reset( new dtPluginFactory() );
    
    //
    // add builder
    //
    _instance->_builder.push_back( new writeStep() );
    _instance->_builder.push_back( new constValueAssingRule() );
    _instance->_builder.push_back( new analyticFunctionToCSV() );
    _instance->_builder.push_back( new volVectorFieldVersusXYZ() );
    _instance->_builder.push_back( new volScalarFieldVersusL() );
    _instance->_builder.push_back( new pOnBlade() );
    _instance->_builder.push_back( new UcylInChannel() );
    _instance->_builder.push_back( new uRelInChannel() );
    _instance->_builder.push_back( new meshQuality() );
    _instance->_builder.push_back( new volScalarInChannelFieldRange() );
    _instance->_builder.push_back( new volVectorPatchFieldRange() );
    _instance->_builder.push_back( new volScalarPatchFieldRange() );
    _instance->_builder.push_back( new volVectorInChannelFieldRange() );
    _instance->_builder.push_back( new volVectorFieldRange() );
    
    return _instance.get();
  }
  
  dtPluginFactory::dtPluginFactory() {   
  }  
}