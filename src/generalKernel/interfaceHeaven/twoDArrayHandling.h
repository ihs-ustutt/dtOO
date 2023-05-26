#ifndef twoDArrayHandling_H
#define	twoDArrayHandling_H

#include <dtOOTypeDef.h>

#include <boost/multi_array.hpp>
#include <logMe/dtMacros.h>
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
      twoDArrayHandling( dtInt const sizeI, dtInt const sizeJ );
      twoDArrayHandling( dtInt const sizeI, dtInt const sizeJ, T const & init );      
      twoDArrayHandling( const twoDArrayHandling& orig );
      virtual ~twoDArrayHandling();
      void resize( dtInt const sizeI, dtInt const sizeJ);
      void resize( dtInt const sizeI);
      dtInt size( dtInt const dim = 0) const;
      std::vector< T > fixJ( dtInt const jj) const;
      std::vector< T > fixI( dtInt const ii) const;
      void clear( void );
  };

  //----------------------------------------------------------------------------
  // cpp
  //----------------------------------------------------------------------------
  template < typename T >
  twoDArrayHandling< T >::twoDArrayHandling() {
  }

  template < typename T >
  twoDArrayHandling< T >::twoDArrayHandling( 
    dtInt const sizeI, dtInt const sizeJ 
  ) 
    : std::vector< std::vector< T > >( sizeI, std::vector< T >(sizeJ) ) {
  }
  
  template < typename T >
  twoDArrayHandling< T >::twoDArrayHandling(
    dtInt const sizeI, dtInt const sizeJ, T const & init
  ) 
    : std::vector< std::vector< T > >( sizeI, std::vector< T >(sizeJ, init) ) {
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
  void twoDArrayHandling< T >::resize( dtInt const sizeI, dtInt const sizeJ) {
    dt__forAllIndex(*this, ii) this->at(ii).clear();        
    this->clear();
    
    std::vector< std::vector< T > >::resize(sizeI, std::vector< T >( sizeJ ) );
  }  

  template < typename T >
  void twoDArrayHandling< T >::resize( dtInt const sizeI ) {
    std::vector< std::vector< T > >::resize(sizeI);
  }
    
  template < typename T >
  dtInt twoDArrayHandling< T >::size( dtInt const dim ) const {
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
  template < typename T >  
  std::vector< T > twoDArrayHandling< T >::fixJ( dtInt const jj ) const {
    std::vector< T > ret(this->size());
    dt__forAllIndex(*this, ii) {
      ret[ii] = this->at(ii).at(jj);      
    }
    return ret;
  }
  
  template < typename T >    
  std::vector< T > twoDArrayHandling< T >::fixI( dtInt const ii ) const {
    return this->at(ii);
  }

  template < typename T >    
  void twoDArrayHandling< T >::clear( void ) {
    dt__forAllIndex(*this, ii) {
      this->at(ii).clear();
    } 
    std::vector< std::vector< T > >::clear();
  }  
}
#endif	/* twoDArrayHandling_H */

