#include "vectorContainer.h"
#include <vector>
#include <logMe/logMe.h>
#include <math.h>

namespace dtOO {  
  vectorContainer::vectorContainer() {
  }

  vectorContainer::vectorContainer(const vectorContainer& orig) {
  }

  vectorContainer::~vectorContainer() {
    _pair.clear();
  }

  void vectorContainer::add(
    dtVector3 const * const vectorP, std::string const * const stringP 
  ) {
    this->add(*vectorP, *stringP);  
  }

  void vectorContainer::add(
    dtVector3 const * const vectorP, std::string const  string 
  ) {
    this->add(*vectorP, string);  
  }

  void vectorContainer::add(
    dtVector3 const vector, std::string const * const stringP 
  ) {
    this->add(vector, *stringP);    
  }

  void vectorContainer::add(
    dtVector3 const vector, std::string const string 
  ) {
    add(vector, string, dtPoint3(0,0,0));
  }

  void vectorContainer::add(
    dtVector3 const * const vector, 
    std::string const string, 
    dtPoint3 const point 
  ) {
    this->add(*vector, string, point);
  }

  void vectorContainer::add(
    dtVector3 const * const vector, 
    std::string const * const string, 
    dtPoint3 const point 
  ) {
    this->add(*vector, *string, point);
  }

  void vectorContainer::add(
    dtVector3 const * const vector, 
    std::string const * const string, 
    dtPoint3 const * const point 
  ) {
    this->add(*vector, *string, *point);
  }

  void vectorContainer::add(
    dtVector3 const vector, 
    std::string const * const string, 
    dtPoint3 const point 
  ) {
    this->add(vector, *string, point);
  }

  void vectorContainer::add(
    dtVector3 const vector, 
    std::string const * const string, 
    dtPoint3 const * const point 
  ) {
    this->add(vector, string, *point);
  }

  void vectorContainer::add(
    dtVector3 const vector, std::string const string, dtPoint3 const point 
  ) {
    if ( string != "" ) {
      for (int ii=0;ii<_pair.size();ii++) {
        if (_pair[ii].second == string) {
          dt__throw(
            add(),
            << "Try to add a vector that is already in container." << std::endl
            << dt__eval(string)
          );
        }
      }
    }
    std::pair< dtVector3, dtPoint3 > inpair( vector, point );
    _pair.push_back( 
      std::pair < std::pair< dtVector3, dtPoint3 >, std::string >(
        inpair, string
      )  
    );  
  }

  dtVector3 vectorContainer::get(std::string const string ) const {
    for (int ii=0;ii<_pair.size();ii++) {
      if (_pair[ii].second == string) {
        return ( _pair[ii].first.first );
      }
    }
    dt__throw(get(), << string << " not in vectorContainer.");
  }


  dtVector3 vectorContainer::get(std::string const * const stringP ) const {
    return ( this->get(*stringP) );
  }

  bool vectorContainer::has(std::string const * const stringP) const {
    return this->has(*stringP);
  }

  bool vectorContainer::has(std::string const string) const {
    for (int ii=0;ii<_pair.size();ii++) {
      if (_pair[ii].second == string) {
        return true;
      }
    }
    return false;
  }

  bool vectorContainer::isEmpty( void ) const {
    return _pair.empty();
  }
}