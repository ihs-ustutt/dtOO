#include "lVHOInterface.h"

#include "lVHOSubject.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/labelHandling.h>
#include <jsonHeaven/jsonPrimitive.h>

namespace dtOO {
  lVHOInterface::lVHOInterface( 
    lVHOSubject * subj, jsonPrimitive const & config 
  ) {
    // attach observer
    subj->attach( this );
    // store config
    _config.reset( new jsonPrimitive( config ) );
    dt__debug( 
      lVHOInterface(),
      << "config() = " << _config->toStdString()
    );
  }

  lVHOInterface::~lVHOInterface( ) {
  
  }

  void lVHOInterface::observeSet( labelHandling * lH ) {
    dt__debug(observeSet(), << "Nothing to do on: " << lH->getLabel()); 
  }

  void lVHOInterface::observe( labelHandling * lH ) {
    dt__debug(observe(), << "Nothing to do on: " << lH->getLabel()); 
  }

  jsonPrimitive & lVHOInterface::config( void ) {
    return *_config;
  }
  
  jsonPrimitive const & lVHOInterface::config( void ) const {
    return *_config;
  }
}
