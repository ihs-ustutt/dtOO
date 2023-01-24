#include "lVHOjsonLoad.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <jsonHeaven/jsonPrimitive.h>
#include <fstream>
#include <constValueHeaven/constValue.h>

namespace dtOO {
  lVHOjsonLoad::lVHOjsonLoad( 
    lVHOSubject * subj, jsonPrimitive const & config
  ) : lVHOInterface(subj, config) {
    // create file stream
    std::fstream fStr 
    = 
    std::fstream(
      this->config().lookup< std::string >("_filename"), std::ios::in
    );

    // search state in jsonPrimitive
    dt__forAllRefAuto(
      jsonPrimitive(fStr).lookupDef< std::vector< jsonPrimitive > >(
        "state", std::vector< jsonPrimitive >()
      ),
      aState
    ) {
      if (
        aState.lookup< std::string >("label")
        ==
        this->config().lookup< std::string>("_label")
      ) {
        _cV.reset(
          new lvH_constValue( 
            jsonPrimitive(aState).lookup< std::vector< constValue * > >("")
          )
        );
      }
    }
    dt__throwIf(!_cV, lVHOjsonLoad());
  }

  lVHOjsonLoad::~lVHOjsonLoad() {
    _cV->destroy();  
  }

  void lVHOjsonLoad::observeSet( labelHandling * lH ) {
    if ( constValue::MustDownCast(lH)->loadable() ) {
      dt__info(
        observeSet(), 
        << "Load constValue[ " << lH->getLabel()  << " ]: set from " 
        << constValue::MustDownCast(lH)->getValue() 
        << " to "
        << _cV->get( lH->getLabel() )->getValue()
      );

      constValue::MustDownCast(lH)->setValue(
        _cV->get( lH->getLabel() )->getValue()
      );
    }
    else {
      dt__info( 
        observeSet(), 
        << "constValue[ " << lH->getLabel()  << " ] not loadable" 
      );
    }
  }
}
