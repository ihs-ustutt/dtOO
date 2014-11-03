#include "pointContainer.h"
#include "interfaceHeaven/ptrHandling.h"
#include <vector>
#include <logMe/logMe.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <discrete3dPoints.h>

namespace dtOO {
  pointContainer::pointContainer() {
    _height 
    = 
    staticPropertiesHandler::getInstance()->getOptionFloat("point_render_diameter");    
  }

  pointContainer::pointContainer(const pointContainer& orig) {
    _height = orig._height;
  }

  pointContainer::~pointContainer() {
    _pair.clear();
  }

  void pointContainer::add( dtPoint3 const * const pointP, std::string const * const stringP ) {
    this->add(*pointP, *stringP);  
  }

  void pointContainer::add( dtPoint3 const * const pointP, std::string const  string ) {
    this->add(*pointP, string);  
  }

  void pointContainer::add( dtPoint3 const point, std::string const * const stringP ) {
    this->add(point, *stringP);    
  }

  void pointContainer::add( dtPoint3 const point, std::string const string, float const heightScale ) {
    if (string != "") {
      for (int ii=0;ii<_pair.size();ii++) {
        if (_pair[ii].second == string) {
          dt__THROW(add(),
                  << "Try to add a point that is already in container." << LOGDEL
                  << DTLOGEVAL(string));
        }
      }
    }
    DTINFOWF(add(),
            << "adding point " << string);
    
    _pair.push_back( std::pair < dtPoint3, std::string >(point, string) );
    _heightScale.push_back( heightScale );
  }

  dtPoint3 pointContainer::get(std::string const string ) const {
    for (int ii=0;ii<_pair.size();ii++) {
      if (_pair[ii].second == string) {
        return ( _pair[ii].first );
      }
    }
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