#include "pointContainer.h"
#include "interfaceHeaven/ptrHandling.h"
#include <vector>
#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <discrete3dPoints.h>

namespace dtOO {
  pointContainer::pointContainer() {
  }

  pointContainer::pointContainer(const pointContainer& orig) {
  }

  pointContainer::~pointContainer() {
    _pair.clear();
  }

  void pointContainer::add( dtPoint3 const point, std::string const string) {
    if (string != "") {
      for (int ii=0;ii<_pair.size();ii++) {
        if (_pair[ii].second == string) {
          dt__throw(
						add(),
            << "Try to add a point that is already in container." << std::endl
            << dt__eval(string)
					);
        }
      }
    }
    dt__debug(add(), << "adding point " << string);
    
    _pair.push_back( std::pair < dtPoint3, std::string >(point, string) );
  }

  dtPoint3 pointContainer::get(std::string const string ) const {
    for (int ii=0;ii<_pair.size();ii++) {
      if (_pair[ii].second == string) {
        return ( _pair[ii].first );
      }
    }
    dt__throw(get(), << string << " not in pointContainer.");
  }

  dtPoint3 pointContainer::get(std::string const * const stringP ) const {
    return ( this->get(*stringP) );
  }

  bool pointContainer::has(std::string const * const stringP) const {
    return this->has(*stringP);
  }

  bool pointContainer::has(std::string const string ) const {
    for (int ii=0;ii<_pair.size();ii++) {
      if (_pair[ii].second == string) {
        return true;
      }
    }
    return false;
  }
  
  bool pointContainer::isEmpty( void ) const {
    return _pair.empty();
  }
}