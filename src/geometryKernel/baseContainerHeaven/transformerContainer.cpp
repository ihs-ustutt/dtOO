#include "transformerContainer.h"
#include <vector>
#include <logMe/logMe.h>
#include <dtTransformerHeaven/dtTransformer.h>

namespace dtOO {
  transformerContainer::transformerContainer() {   
  }

  transformerContainer::transformerContainer(const transformerContainer& orig) {
  }

  transformerContainer::~transformerContainer() {
    _dtT.destroy();
  }

  void transformerContainer::add(dtTransformer const * const dtT ) {
    dt__info( add(), << "Add transformer " << dtT->getLabel() );
    _dtT.push_back( dtT->clone() );
  }

  dtTransformer const * transformerContainer::get( std::string const string ) const {
    return _dtT.get(string);
  }

  bool transformerContainer::has(std::string const string) const {
    return _dtT.has(string);
  }

  bool transformerContainer::isEmpty( void ) const {
    return _dtT.empty();
  }
}