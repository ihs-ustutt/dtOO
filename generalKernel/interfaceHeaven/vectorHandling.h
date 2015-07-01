#ifndef VECTORHANDLING_H
#define	VECTORHANDLING_H

#include <vector>
#include <string>
#include "labelHandling.h"
#include <progHelper.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include "stringPrimitive.h"

namespace dtOO {
  //----------------------------------------------------------------------------
  // h
  //----------------------------------------------------------------------------
  template < typename T >
  class vectorHandling : public std::vector< T >{
  public:
    typedef typename std::vector< T >::iterator iterator;    
  public:
    dt__classOnlyName(vectorHandling);
    vectorHandling();
    vectorHandling(const vectorHandling& orig);
    vectorHandling(const vectorHandling& orig0, const vectorHandling& orig1);        
    vectorHandling(const vectorHandling& orig0, const vectorHandling& orig1, const vectorHandling& orig2);    
    vectorHandling(const std::vector< T >& orig);    
    vectorHandling(int const dim);
    virtual ~vectorHandling();
    T * set( T const & toSet);
    T * set( T const * toSet);
    T const & get( int const ii) const;
    T const & get( std::string const label) const;
    std::string getLabel( int const pos ) const;
    T & getRef( std::string const label);
    bool has( std::string const label) const;
    bool hasTwice( std::string const label ) const;
    void checkForBastardTwins( void ) const;
    void checkForBastardTwinsAndMakeUnique( void );
    void nullify( void );
    int getPosition( std::string const label) const;
    vectorHandling< T >::iterator getIterator( int const pos );
    void destroy( void );
    void destroy( std::vector< T > & vec );
    void erase( int const pos );
    void addIndex( void );
    void dump(void);
  };

  //----------------------------------------------------------------------------
  // cpp
  //----------------------------------------------------------------------------
  template < typename T >
  vectorHandling< T >::vectorHandling() {
  }

  template < typename T >
  vectorHandling< T >::vectorHandling(const vectorHandling& orig) : std::vector< T >(orig.size()) {
    dt__forAllIndex(orig, ii) this->at(ii) = orig[ii];
  }
  
  template < typename T >
  vectorHandling< T >::vectorHandling(const vectorHandling& orig0, const vectorHandling& orig1) 
    : std::vector< T >(orig0.size()+orig1.size()) {
    int counter = 0;
    dt__forAllIndex(orig0, ii) {
      this->at(counter) = orig0[ii];
      counter++;
    }
    dt__forAllIndex(orig1, ii) {
      this->at(counter) = orig1[ii];
      counter++;
    }
  }
  
  template < typename T >
  vectorHandling< T >::vectorHandling(const vectorHandling& orig0, const vectorHandling& orig1, const vectorHandling& orig2) 
    : std::vector< T >(orig0.size()+orig1.size()+orig2.size()) {
    int counter = 0;
    dt__forAllIndex(orig0, ii) {
      this->at(counter) = orig0[ii];
      counter++;
    }
    dt__forAllIndex(orig1, ii) {
      this->at(counter) = orig1[ii];
      counter++;
    }
    dt__forAllIndex(orig2, ii) {
      this->at(counter) = orig2[ii];
      counter++;
    }
  }
  
  template < typename T >
  vectorHandling< T >::vectorHandling(const std::vector< T >& orig) : std::vector< T >(orig.size()) {
    dt__forAllIndex(orig, ii) {
      this->at(ii) = orig[ii];
    }
  }

  template < typename T >
  vectorHandling< T >::vectorHandling(int const dim) : std::vector<T>(dim) {
    
  }
  
  template < typename T >
  vectorHandling< T >::~vectorHandling() {
  }
  
  template< typename T >
  T * vectorHandling< T >::set( T const & toSet) {
    this->push_back( toSet );
    return &( this->back() );
  }
  
  template< typename T >
  T * vectorHandling< T >::set( T const * toSet) {
    this->push_back( *toSet );
    return &( this->back() );
  }
  
  template< typename T >
  T const & vectorHandling< T >::get( int const ii) const {
    return this->at(ii);
  }

  template< typename T >
  T & vectorHandling< T >::getRef( std::string const label ) {
    return this->at( this->getPosition(label) );
  }  

  template< typename T >
  T const & vectorHandling< T >::get( std::string const label ) const {
    dt__forAllIndex(*this, ii) {
      //
      // check if class is of type labelHandling
      //
      labelHandling const * obj;
      dt__mustCast(this->at(ii), labelHandling const, obj);
      
      if (obj->getLabel() == label ) {
        return this->at(ii);
      }
    }
  dt__throw(get(), << "No element with " << dt__eval(label) );
  }

  template< typename T >
  std::string vectorHandling< T >::getLabel( int const pos ) const {
    labelHandling const * obj 
    = 
    dynamic_cast< labelHandling const * >(this->at(pos));

    if (obj ) {
      return obj->getLabel();
    }
    else {
      dt__throw(
        getLabel(), 
        << "No labelHandling at " << dt__eval(pos) << " in vector."
      );
    }
  }  
  
  template< typename T >
  bool vectorHandling< T >::has( std::string const label ) const {
    dt__forAllIndex(*this, ii) {
      //
      // check if class is of type labelHandling
      //
      labelHandling const * obj;
      dt__mustCast(this->at(ii), labelHandling const, obj);
      
      if (obj->getLabel() == label ) {
        return true;
      }
    }
    return false;
  }

  template< typename T >
  bool vectorHandling< T >::hasTwice( std::string const label ) const {
    int counter = 0;
    dt__forAllIndex(*this, ii) {
      //
      // check if class is of type labelHandling
      //
      labelHandling const * obj;
      dt__mustCast(this->at(ii), labelHandling const, obj);
      
      if (obj->getLabel() == label ) {
        counter++;
      }
      if (counter == 2) {
        return true;
      }
    }
    return false;
  }
  
  template< typename T >
  void vectorHandling< T >::checkForBastardTwins( void ) const {
    dt__forAllIndex(*this, ii) {
      //
      // check if class is of type labelHandling
      //
      labelHandling const * obj;
      dt__mustCast(this->at(ii), labelHandling const, obj);
    
      //
      // check
      //
      if ( this->hasTwice( obj->getLabel() ) ) {
        dt__throw(checkForBastardTwins(),
                 << "Duplicate element " << dt__eval( obj->getLabel() ) );
      }
    }
  }

  template< typename T >
  void vectorHandling< T >::checkForBastardTwinsAndMakeUnique( void ) {
    dt__forAllIndex(*this, ii) {
      //
      // check if class is of type labelHandling
      //
      labelHandling * obj;
      dt__mustCast(this->at(ii), labelHandling, obj);
    
      //
      // check
      //
      if ( this->hasTwice( obj->getLabel() ) ) {
        dt__warning(checkForBastardTwinsAndMakeUnique(),
                << "Make label " << dt__eval(obj->getLabel()) << " unique!");          
        obj->setLabel( obj->getLabel()+"_" );
      }
    }
  }  

  template< typename T >
  void vectorHandling< T >::nullify( void ) {
    dt__forAllIndex(*this, ii) this->at(ii) = NULL;
  }  
  
  template< typename T >
  int vectorHandling< T >::getPosition( std::string const label ) const {
    dt__forAllIndex(*this, ii) {
      //
      // check if class is of type labelHandling
      //
      labelHandling const * obj = dynamic_cast< labelHandling const * >(
                              this->at(ii)
                            );
      if (obj->getLabel() == label ) {
        return ii;
      }
    }
    return -1;
  }  

  template< typename T >
  typename vectorHandling< T >::iterator vectorHandling< T >::getIterator( int const pos ) {
    typename vectorHandling< T >::iterator it;
    int counter = 0;
    for (it = std::vector<T>::begin(); it != std::vector<T>::end(); ++it) {
//      labelHandling const * obj;
//      dt__CANDOWNCAST(*it, labelHandling const, obj);
//      if (obj) {
//        dt__info( getIterator(), << dt__eval(obj->getLabel()) );
//      }      
      if (counter == pos) {
        return it;
      }
      counter++;
    }
    dt__throw(getIterator(), << "No position " << dt__eval(pos) << " in vector." );
  }    

  /**
   * @todo check if T is a pointer
   */
  template< typename T >
  void vectorHandling< T >::destroy( void ) {
    dt__forAllIndex(*this, ii) delete this->at(ii);
    this->clear();
  }

  template< typename T >
  void vectorHandling< T >::destroy( std::vector< T > & vec ) {
    dt__forAllIndex(vec, ii) delete vec.at(ii);
    vec.clear();
  }
  
  template< typename T >
  void vectorHandling< T >::erase( int const pos ) {
    typename vectorHandling< T >::iterator it = getIterator(pos);
    std::vector<T>::erase(it);
  }
  
  template< typename T >
  void vectorHandling< T >::addIndex( void ) {
    dt__forAllIndex(*this, ii) {
      //
      // check if class is of type labelHandling
      //
      labelHandling * obj;
      dt__mustCast(this->at(ii), labelHandling, obj);
      
      obj->setLabel( stringPrimitive().intToString(ii)+"_"+obj->getLabel() );
    }
  }
  template< typename T >  
  void vectorHandling< T >::dump(void) {
    std::vector< std::string > header;
    header.push_back("label");
    header.push_back("type");
    std::vector< std::string > itVal;
    dt__forAllIndex(*this, ii) {
      labelHandling * obj;
      dt__mustCast(this->at(ii), labelHandling, obj);
      
      itVal.push_back( obj->getLabel());
      itVal.push_back( obj->virtualClassName() );
    }
    
    dt__info(dump(), << logMe::vecToTable(header, itVal) );
  }
}
#endif	/* VECTORHANDLING_H */

