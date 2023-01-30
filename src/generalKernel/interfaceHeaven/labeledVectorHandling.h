#ifndef labeledVectorHandling_H
#define	labeledVectorHandling_H

#include <algorithm>
#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "logMe/logMe.h"
#include "vectorHandling.h"
#include "lVHOSubject.h"
#include "lVHOInterface.h"
#include "stringPrimitive.h"

namespace dtOO {
  //----------------------------------------------------------------------------
  // h
  //----------------------------------------------------------------------------
  template < typename T >
  class labeledVectorHandling : public vectorHandling< T >, public lVHOSubject {
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
      void set( T const & toSet);
      void set( T const * toSet);
      const_reference get( std::string const & label) const;
      std::vector< dtInt > getIndices( std::string const & label) const;
      std::string getLabel( dtInt const pos ) const;
      std::vector< std::string > labels( void ) const;
      bool has( std::string const label) const;
      dtInt getPosition( std::string const label) const;
      void dump(void) const;
      void sort(void);
      const_reference operator[](std::string const & label) const;
      reference operator[](int ii) { 
        return std::vector< T >::at(ii); 
      }
      const_reference operator[](int ii) const { 
        return std::vector< T >::at(ii); 
      }
      virtual void attach(lVHOInterface * observer);
      virtual void letObserve();
    private:
      dt__pVH(lVHOInterface) _observers;
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
    dt__throwIf( !orig._observers.empty(), labeledVectorHandling() );
  }
  
  template < typename T >
  labeledVectorHandling< T >::labeledVectorHandling(
    const labeledVectorHandling& orig0, const labeledVectorHandling& orig1
  ) : vectorHandling< T >(orig0, orig1) {
    dt__throwIf( !orig0._observers.empty(), labeledVectorHandling() );
    dt__throwIf( !orig1._observers.empty(), labeledVectorHandling() );

  }
  
  template < typename T >
  labeledVectorHandling< T >::labeledVectorHandling(
    const labeledVectorHandling& orig0, 
    const labeledVectorHandling& orig1, 
    const labeledVectorHandling& orig2
  ) : vectorHandling< T >(orig0, orig1, orig2) {
    dt__throwIf( !orig0._observers.empty(), labeledVectorHandling() );
    dt__throwIf( !orig1._observers.empty(), labeledVectorHandling() );
    dt__throwIf( !orig2._observers.empty(), labeledVectorHandling() );
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
  void labeledVectorHandling< T >::set( T const & toSet) {
    std::vector<T>::push_back( toSet );
    dt__forAllRefAuto( _observers, anObserver ) {
      anObserver.observeSet( this->back() );
    }
  }
  
  template< typename T >
  void labeledVectorHandling< T >::set( T const * toSet) {
    std::vector< T >::push_back( *toSet );
    dt__forAllRefAuto( _observers, anObserver ) {
      anObserver.observeSet( this->back() );
    }
  }

  template< typename T >
  typename labeledVectorHandling< T >::const_reference 
  labeledVectorHandling< T >::get( std::string const & label ) const {
    dtInt pos = this->getPosition( label );
    dt__throwIfWithMessage(pos==-1, get(), << label << " not found!");
    return this->at( pos );
  }

  template< typename T >
  std::vector< dtInt >
  labeledVectorHandling< T >::getIndices( std::string const & label ) const {
    std::vector< dtInt > ret;
    dtInt cc = 0;
    dt__forAllIndex(*this, ii) {
      //
      // check if class is of type labelHandling
      //
      labelHandling const * obj;
      dt__mustCast(this->at(ii), labelHandling const, obj);
      
      if (stringPrimitive::matchWildcard( label, obj->getLabel() ) ) {
        ret.push_back( cc );
      }
      cc = cc + 1;
    }
    dt__throwIfWithMessage(ret.empty(), getIndices(), << "No elements for " << label);
    return ret;
  }


  template< typename T >
  std::string labeledVectorHandling< T >::getLabel( dtInt const pos ) const {
    labelHandling const * obj;
    dt__mustCast(this->at(pos), labelHandling const, obj);
    return obj->getLabel();
  }  
  
  template< typename T >
  std::vector< std::string > labeledVectorHandling< T >::labels( void ) const {
    std::vector< std::string > ret;
    dt__forAllIndex(*this, ii) {
      ret.push_back( this->getLabel( ii ) );
    }
    return ret;
  }  
  
  template< typename T >
  bool labeledVectorHandling< T >::has( std::string const label ) const {
    if ( this->getPosition( label ) < 0 ) {
      return false;
    }
    return true;
  }

  template< typename T >
  dtInt labeledVectorHandling< T >::getPosition( std::string const label ) const {
    dt__forAllIndex(*this, ii) {
      //
      // check if class is of type labelHandling
      //
      labelHandling const * obj 
      = 
      dynamic_cast< labelHandling const * >( this->at(ii) );
      if (obj->getLabel() == label ) {
        return ii;
      }
    }
    return -1;
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

  template< typename T >
  void labeledVectorHandling< T >::attach(lVHOInterface * observer) {
    _observers.push_back( observer );
  }
  
  template< typename T >
  void labeledVectorHandling< T >::letObserve( void ) {
    dt__forAllRefAuto( _observers, anObserver ) {
      dt__forAllIndex(*this, ii) {
        anObserver.observe( this->at(ii) );
      }
    }
  }
}
#endif	/* labeledVectorHandling_H */

