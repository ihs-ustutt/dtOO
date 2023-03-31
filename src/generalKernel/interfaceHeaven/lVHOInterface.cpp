#include "lVHOInterface.h"

#include "lVHOSubject.h"
#include <logMe/logMe.h>
#include <interfaceHeaven/labelHandling.h>
#include <jsonHeaven/jsonPrimitive.h>

namespace dtOO {
  lVHOInterface::lVHOInterface( 
    jsonPrimitive const & config, lVHOSubject * subj
  ) {
    _subj = subj;
    // attach observer
    if (subj!=NULL) subj->attach( this );
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

  lVHOSubject & lVHOInterface::subject( void ) {
    dt__throwIf(_subj==NULL, subject());
    return *_subj;
  }
  
  lVHOSubject const & lVHOInterface::subject( void ) const {
    dt__throwIf(_subj==NULL, subject());
    return *_subj;
  }
}
