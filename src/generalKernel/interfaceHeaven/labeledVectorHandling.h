#ifndef labeledVectorHandling_H
#define	labeledVectorHandling_H

#include <dtOOTypeDef.h>

#include "vectorHandling.h"

namespace dtOO {
  //----------------------------------------------------------------------------
  // h
  //----------------------------------------------------------------------------
  template < typename T >
  class labeledVectorHandling : public vectorHandling< T > {
//    static_assert(
//      std::is_base_of< labelHandling, *T >::value, 
//      "labelHandling must be a base of T"
//    );
  public:
    typedef typename std::vector< T >::iterator iterator;
    typedef typename std::vector< T >::reference reference;
    typedef typename std::vector< T >::const_reference const_reference;
  public:
    dt__classOnlyName(labeledVectorHandling);
    labeledVectorHandling();
    labeledVectorHandling(const labeledVectorHandling& orig);
    labeledVectorHandling(
      const labeledVectorHandling& orig0, const labeledVectorHandling& orig1
    );        
    labeledVectorHandling(
      const labeledVectorHandling& orig0, 
      const labeledVectorHandling& orig1, 
      const labeledVectorHandling& orig2
    );    
    labeledVectorHandling(const std::vector< T >& orig);    
    labeledVectorHandling(int const dim);
    labeledVectorHandling(int const dim, T init);
    virtual ~labeledVectorHandling();
    const_reference get( std::string const & label) const;
    reference get( std::string const & label);    
    std::string getLabel( dtInt const pos ) const;
    std::vector< std::string > labels( void ) const;
    bool has( std::string const label) const;
    bool hasTwice( std::string const label ) const;
    void checkForBastardTwins( void ) const;
    void checkForBastardTwinsAndMakeUnique( void );
    dtInt getPosition( std::string const label) const;
    void addIndex( void );
    void dump(void) const;
    void sort(void);
    const_reference operator[](std::string const & label) const;
    reference operator[](int ii) { 
      return std::vector< T >::at(ii); 
    }
    const_reference operator[](int ii) const { 
      return std::vector< T >::at(ii); 
    }
  };

  //----------------------------------------------------------------------------
  // cpp
  //----------------------------------------------------------------------------
  template < typename T >
  labeledVectorHandling< T >::labeledVectorHandling() : vectorHandling< T >() {

  }

  template < typename T >
  labeledVectorHandling< T >::labeledVectorHandling(
    const labeledVectorHandling& orig
  ) : vectorHandling< T >(orig) {

  }
  
  template < typename T >
  labeledVectorHandling< T >::labeledVectorHandling(
    const labeledVectorHandling& orig0, const labeledVectorHandling& orig1
  ) : vectorHandling< T >(orig0, orig1) {

  }
  
  template < typename T >
  labeledVectorHandling< T >::labeledVectorHandling(
    const labeledVectorHandling& orig0, 
    const labeledVectorHandling& orig1, 
    const labeledVectorHandling& orig2
  ) : vectorHandling< T >(orig0, orig1, orig2) {

  }
  
  template < typename T >
  labeledVectorHandling< T >::labeledVectorHandling(
    const std::vector< T >& orig
  ) : vectorHandling< T >( orig) {

  }

  template < typename T >
  labeledVectorHandling< T >::labeledVectorHandling(int const dim) 
    : vectorHandling<T>(dim) {

  }
  
  template < typename T >
  labeledVectorHandling< T >::labeledVectorHandling( dtInt const dim, T init ) 
    : vectorHandling<T>(dim, init) {
    
  }
  
  template < typename T >
  labeledVectorHandling< T >::~labeledVectorHandling() {
  }
  
  template< typename T >
  typename labeledVectorHandling< T >::const_reference 
  labeledVectorHandling< T >::get( std::string const & label ) const {
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
  typename labeledVectorHandling< T >::reference 
  labeledVectorHandling< T >::get( std::string const & label ) {
    return this->at( this->getPosition(label) );
  }  
  
  template< typename T >
  std::string labeledVectorHandling< T >::getLabel( dtInt const pos ) const {
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
  std::vector< std::string > labeledVectorHandling< T >::labels( void ) const {
    std::vector< std::string > ret;
    dt__forAllIndex(*this, ii) {
      labelHandling const * obj;
      dt__mustCast(this->at(ii), labelHandling const, obj);
      ret.push_back( obj->getLabel() );
    }
    return ret;
  }  
  
  template< typename T >
  bool labeledVectorHandling< T >::has( std::string const label ) const {
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
  bool labeledVectorHandling< T >::hasTwice( std::string const label ) const {
    dtInt counter = 0;
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
  void labeledVectorHandling< T >::checkForBastardTwins( void ) const {
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
  void labeledVectorHandling< T >::checkForBastardTwinsAndMakeUnique( void ) {
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
  dtInt labeledVectorHandling< T >::getPosition( std::string const label ) const {
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
  void labeledVectorHandling< T >::addIndex( void ) {
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
  void labeledVectorHandling< T >::dump(void) const {
    logContainer< labeledVectorHandling< T > > logC(logINFO, "dump()");
    logC() 
      << logMe::dtFormat("[ %40s ] -> %s") % "label" % "type" 
      << std::endl;
    std::vector< std::string > itVal;
    dt__forAllIndex(*this, ii) {
      labelHandling * obj;
      dt__mustCast(this->at(ii), labelHandling, obj);
      
      logC() 
        << logMe::dtFormat("[ %40s ] -> %s") 
          % obj->getLabel() % obj->virtualClassName()
        << std::endl;
    }
  }
  
  template< typename T >  
  void labeledVectorHandling< T >::sort(void) {
    std::vector< std::string > L;
    dt__forAllIndex(*this, ii) {
      labelHandling * obj;
      dt__mustCast(this->at(ii), labelHandling, obj);
      L.push_back( obj->getLabel() );
    }
    std::sort( L.begin(), L.end() );
    dt__forAllIndex(L, ii) {
      dtInt pos = this->getPosition( L[ii] );
      std::iter_swap( this->begin() + pos, this->begin() + ii);
    }    
  }
  
  template< typename T >
  typename labeledVectorHandling< T >::const_reference 
  labeledVectorHandling< T >::operator[](std::string const & label) const {
    return this->get(label);
  } 
}
#endif	/* labeledVectorHandling_H */

