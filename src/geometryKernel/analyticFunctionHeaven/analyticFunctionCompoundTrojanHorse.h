#ifndef analyticFunctionCompoundTrojanHorse_H
#define	analyticFunctionCompoundTrojanHorse_H

#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  template < typename funT >
  class analyticFunctionCompoundTrojanHorse {
    public:
      dt__classOnlyName(analyticFunctionCompoundTrojanHorse);
      analyticFunctionCompoundTrojanHorse();
      analyticFunctionCompoundTrojanHorse( 
        funT const * const _refPtr,
        vectorHandling< analyticFunction * > const & vec, 
        std::map< int, int > const & pos_tag
      );    
      ~analyticFunctionCompoundTrojanHorse();
      funT * component( int const & pos) const;
      funT * addComponent( funT const * const toAdd);
      funT * addComponent( funT const * const toAdd, int const & tag);
      int nComponents( void ) const;
      funT * componentFromTag( int const & tag) const;
      bool hasTag( int const & tag) const;
      funT const * constRefPtr( void ) const;
    private:
      funT const * _refPtr;
      vectorHandling< analyticFunction * > * _vec;
      std::map< int, int > * _pos_tag;
  };  
  
  template < typename funT >
  analyticFunctionCompoundTrojanHorse< funT >
    ::analyticFunctionCompoundTrojanHorse() {
    _vec = NULL;
    _pos_tag = NULL;
  }
  
  template < typename funT >  
  analyticFunctionCompoundTrojanHorse< funT >
    ::analyticFunctionCompoundTrojanHorse(
    funT const * const refPtr,
    vectorHandling< analyticFunction * > const & vec, 
    std::map< int, int > const & pos_tag
  ) {
    _refPtr = refPtr;
    _vec = const_cast< vectorHandling< analyticFunction * > * >(&vec);
    _pos_tag = const_cast< std::map< int, int > * >(&pos_tag);
  }  
  
  template < typename funT >  
  analyticFunctionCompoundTrojanHorse< funT >
    ::~analyticFunctionCompoundTrojanHorse() {
  }
     
  template < typename funT >  
  funT *
  analyticFunctionCompoundTrojanHorse< funT >
    ::component( int const & pos ) const {
    return funT::DownCast( _vec->at(pos) );
  }
  
  template < typename funT >  
  funT * analyticFunctionCompoundTrojanHorse< funT >
    ::addComponent(funT const * const toAdd ) {
    _vec->push_back( toAdd->clone() );
    
    int tag = 0;
    if ( !_pos_tag->empty() ) tag = _pos_tag->rbegin()->first + 1;
    _pos_tag->operator[]( tag ) = _vec->size()-1;    

    return funT::DownCast( _vec->back() );
  }

  template < typename funT >  
  funT * analyticFunctionCompoundTrojanHorse< funT >
    ::addComponent(funT const * const toAdd, int const & tag ) {
    dt__throwIf( hasTag(tag), addComponent() );
    _vec->push_back( toAdd->clone() );
    
//    int tag = 0;
//    if ( !_pos_tag->empty() ) tag = _pos_tag->rbegin()->first + 1;
    _pos_tag->operator[]( tag ) = _vec->size()-1;    

    return funT::DownCast( _vec->back() );
  }
  
  template < typename funT >  
  int analyticFunctionCompoundTrojanHorse< funT >::nComponents( void ) const {
    return _vec->size();
  }
  
  template < typename funT >  
  funT *
  analyticFunctionCompoundTrojanHorse< funT >
    ::componentFromTag( int const & tag) const {
    return component( _pos_tag->at( tag ) );
  }
  
  template < typename funT >  
  bool analyticFunctionCompoundTrojanHorse< funT >
    ::hasTag( int const & tag) const {
    return ( _pos_tag->find( tag ) != _pos_tag->end() );
  }  

  template < typename funT >  
  funT const * 
  analyticFunctionCompoundTrojanHorse< funT >::constRefPtr( void ) const {
    return _refPtr;
  }
}
#endif	/* analyticFunctionCompoundTrojanHorse_H */

