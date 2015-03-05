#ifndef ANALYTICFUNCTIONCOMPOUND_H
#define	ANALYTICFUNCTIONCOMPOUND_H

#include <interfaceHeaven/vectorHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include "analyticFunction.h"

namespace dtOO {
  template < typename funT >
  class analyticFunctionCompound : public funT {
  public:
    dt__CLASSSTD(analyticFunctionCompound, analyticFunction); 
    analyticFunctionCompound();
    analyticFunctionCompound( analyticFunctionCompound const & orig);  
    virtual ~analyticFunctionCompound();
    virtual analyticFunctionCompound * clone( void ) const;
    virtual analyticFunctionCompound * create( void ) const;     
    funT const & component( int const & pos) const;
    funT & addComponent( funT const & toAdd);
    int nComponents( void ) const;
    virtual vectorHandling< renderInterface * > getRender( void ) const;
   	virtual bool isCompound( void ) const;
    vectorHandling< analyticFunction const * > compoundInternal( void ) const;    
  private:
    vectorHandling< funT > _vec;
  };  
  
  template < typename funT >
  analyticFunctionCompound< funT >::analyticFunctionCompound() : funT() {
    
  }

  template < typename funT >  
  analyticFunctionCompound< funT >::analyticFunctionCompound(analyticFunctionCompound const & orig) : funT() {
    for (int ii=0; ii<orig._vec.size(); ii++) {
      _vec.push_back( funT(orig._vec[ii]) );  
    }
  }  

  template < typename funT >  
  analyticFunctionCompound< funT >::~analyticFunctionCompound() {
    
  }
  
  template < typename funT >  
  analyticFunctionCompound< funT > * analyticFunctionCompound< funT >::clone( void ) const {
    return new analyticFunctionCompound< funT >(*this);
  }
  
  template < typename funT >  
  analyticFunctionCompound< funT > * analyticFunctionCompound< funT >::create( void ) const {
    return new analyticFunctionCompound< funT >();
  }
    
  template < typename funT >  
  funT const & analyticFunctionCompound< funT >::component( int const & pos ) const {
    return _vec[pos];
  }
  
  template < typename funT >  
  funT & analyticFunctionCompound< funT >::addComponent(funT const & toAdd ) {
    _vec.push_back( funT(toAdd) );
    
    return _vec.back();
  }

  template < typename funT >  
  int analyticFunctionCompound< funT >::nComponents( void ) const {
    return _vec.size();
  }
  
  template < typename funT >  
  vectorHandling< renderInterface * > analyticFunctionCompound< funT >::getRender( void ) const {
    vectorHandling< renderInterface * > retVec;
    dt__FORALL(_vec, ii,
      vectorHandling< renderInterface * > tVec = _vec[ii].getRender();
      dt__FORALL( tVec, jj, retVec.push_back( tVec[jj] ); );
    );
    
    return retVec;
  }

  template < typename funT >  
  bool analyticFunctionCompound< funT >::isCompound( void ) const {
		return true;
	}
  
  template < typename funT >    
	vectorHandling< analyticFunction const * > analyticFunctionCompound< funT >::compoundInternal( void ) const {
		vectorHandling< analyticFunction const * > aFV(_vec.size());
    dt__FORALL(_vec, ii,
      aFV[ii] = analyticFunction::ConstDownCast( &(_vec[ii]) );
    );
    
    return aFV;
	}    
}
#endif	/* ANALYTICFUNCTIONCOMPOUND_H */

