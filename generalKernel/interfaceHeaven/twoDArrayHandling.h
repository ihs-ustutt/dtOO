#ifndef twoDArrayHandling_H
#define	twoDArrayHandling_H

#include <boost/multi_array.hpp>
#include <progHelper.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
  //----------------------------------------------------------------------------
  // h
  //----------------------------------------------------------------------------
  template < typename T >
  class twoDArrayHandling : public std::vector< std::vector< T > > {
    public:
      typedef typename std::vector< std::vector< T >  >::iterator iterator0;
      typedef typename std::vector< std::vector< T >  >::const_iterator const_iterator0;
      typedef typename std::vector< T >::iterator iterator1;
      typedef typename std::vector< T >::const_iterator const_iterator1;      
    public:
      dt__classOnlyName(twoDArrayHandling);
      twoDArrayHandling();
      twoDArrayHandling( int const sizeI, int const sizeJ );
      twoDArrayHandling(const twoDArrayHandling& orig);
      virtual ~twoDArrayHandling();
      void resize(int const sizeI, int const sizeJ);
      void resize(int const sizeI);
      int size( int const dim = 0) const;
  };

  //----------------------------------------------------------------------------
  // cpp
  //----------------------------------------------------------------------------
  template < typename T >
  twoDArrayHandling< T >::twoDArrayHandling() {
  }

  template < typename T >
  twoDArrayHandling< T >::twoDArrayHandling(int const sizeI, int const sizeJ) 
    : std::vector< std::vector< T > >( sizeI, std::vector< T >(sizeJ) ) {
  }
  
  template < typename T >
  twoDArrayHandling< T >::twoDArrayHandling(const twoDArrayHandling& orig) {
    resize(orig.size(0), orig.size(1));
    
    dt__forAllIndex(orig, ii) {
      dt__forAllIndex(orig[ii], jj) {      
        this->at(ii).at(jj) = T(orig[ii][jj]);
      }
    }    
  }

  template < typename T >
  twoDArrayHandling< T >::~twoDArrayHandling() {
  }

  template < typename T >
  void twoDArrayHandling< T >::resize(int const sizeI, int const sizeJ) {
    dt__forAllIndex(*this, ii) this->at(ii).clear();        
    this->clear();
    
    std::vector< std::vector< T > >::resize(sizeI, std::vector< T >( sizeJ ) );
  }  

  template < typename T >
  void twoDArrayHandling< T >::resize(int const sizeI) {
    std::vector< std::vector< T > >::resize(sizeI);
  }
    
  template < typename T >
  int twoDArrayHandling< T >::size(int const dim) const {
    if (dim == 0) {
      return std::vector< std::vector< T > >::size();
    }
    else if (dim == 1) {
      if (size() > 0) {
        return this->at(0).size();
      }
      else {
        return 0;
      }
    }
    else {
      dt__throw( size(), << "Wrong dimension " << dt__eval(dim) );
    }
  }    
}
#endif	/* twoDArrayHandling_H */

