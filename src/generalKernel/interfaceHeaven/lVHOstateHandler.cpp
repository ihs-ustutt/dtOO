#include "lVHOstateHandler.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <jsonHeaven/jsonPrimitive.h>
#include <fstream>
#include <constValueHeaven/constValue.h>
#include <progHelper.h>

namespace dtOO {
  lVHOSubject * lVHOstateHandler::_subj(NULL);

  lVHOstateHandler::lVHOstateHandler( void ) 
  : 
  lVHOInterface( jsonPrimitive() ) {
    dt__throwIfWithMessage(
      !lVHOstateHandler::initialized(),
      lVHOstateHandler(),
      << 
        "No lVHOstateHandler() initialized. Please initialize this observer "
        "to the desired labeledVectorHandling< constValue * > object."
    );
  }
  
  lVHOstateHandler::lVHOstateHandler( 
    jsonPrimitive const & config, lVHOSubject * subj
  ) : lVHOInterface(config, subj) {
    if ( lVHOstateHandler::initialized() ) {
      dt__throw(lVHOstateHandler(), << "Already initialized.");
    }
    _subj = subj;
  }

  lVHOstateHandler::~lVHOstateHandler() {

  }

  bool lVHOstateHandler::initialized( void ) {
    return ( lVHOstateHandler::_subj!=NULL );
  }

  std::string lVHOstateHandler::commonState( void ) {
    std::vector< std::string > cStates;
    
    dt__forAllRefAuto( 
      constValue::VectorMustDownCast( this->subject().internalStdVector() ), cV  
    ) {
      if ( cV->loadable() ) cStates.push_back( cV->getState() );
    }
    progHelper::removeBastardTwins(cStates);

    dt__debug(commonState(), << cStates);

    if (cStates.size() == 1) {
      return cStates[0];
    }
    else {
      return std::string();
    }
  }

  void lVHOstateHandler::makeState( std::string const & label ) {
    std::vector< constValue * > cVs
    =
    constValue::VectorMustDownCast( this->subject().internalStdVector() );

    dt__forAllRefAuto(cVs, cV) if ( cV->loadable() ) cV->setState( label );
  }

  std::string lVHOstateHandler::writeState( std::string label ) {
    if ( label == "" ) {
      label = NowDateAndTime();
      this->makeState( label );
    }
    
    std::vector< constValue * > cV
    =
    constValue::VectorMustDownCast( this->subject().internalStdVector() );

    jsonPrimitive().append< std::vector< jsonPrimitive > >(
      "state",
      std::vector< jsonPrimitive >(
        1, 
        jsonPrimitive()
          .append< std::string >("label", label)
          .append< std::vector< constValue * > >("", cV)
      )
    ).write( label+".json" ); 

    return label;
  }

  lVHOSubject & lVHOstateHandler::subject( void ) {
    dt__throwIf(lVHOstateHandler::_subj==NULL, subject());
    return *lVHOstateHandler::_subj;
  }
  
  lVHOSubject const & lVHOstateHandler::subject( void ) const {
    dt__throwIf(lVHOstateHandler::_subj==NULL, subject());
    return *lVHOstateHandler::_subj;
  }
 
  bool lVHOstateHandler::typeObserves( lVHOSubject const & subj ) const {
    dt__forAllRefAuto( subj.refObserver(), obs ) {
      if ( lVHOstateHandler::Is( &obs ) ) return true;
    }
    return false;
  }

  void lVHOstateHandler::clear( void ) {
    lVHOstateHandler::_subj = NULL;  
  }
}
