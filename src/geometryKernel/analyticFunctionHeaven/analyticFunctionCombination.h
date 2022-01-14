#ifndef analyticFunctionCombination_H
#define	analyticFunctionCombination_H

#include <dtOOTypeDef.h>

#include "vec3dThreeD.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  class dtTransformer;
  
  template < typename funT, typename invFunT >
  class analyticFunctionCombination : public funT {
  public:
    dt__class(analyticFunctionCombination, analyticFunction);     
    analyticFunctionCombination();
    analyticFunctionCombination(analyticFunctionCombination const & orig);
    analyticFunctionCombination(
      funT const * const fun, invFunT const * const invFun
    );
    analyticFunctionCombination * clone( void ) const;
    virtual analyticFunctionCombination * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;          
    analyticFunctionCombination * create( void ) const;    
    virtual ~analyticFunctionCombination();
    virtual aFY Y( aFX const & xx ) const;
    virtual aFX invY( aFY const & yy ) const;
    virtual bool closed( int const & dir ) const;
  private:
    dt__pH(funT) _fun;
    dt__pH(invFunT) _invFun;
  };
  
  template < typename funT, typename invFunT >
  analyticFunctionCombination< funT, invFunT >
    ::analyticFunctionCombination(
  ) {
    
  }
  
  template < typename funT, typename invFunT >
  analyticFunctionCombination< funT, invFunT >
    ::analyticFunctionCombination(
      analyticFunctionCombination const & orig
  ) : funT(orig) {
    _fun.reset( orig._fun->clone() );
    _invFun.reset( orig._invFun->clone() );    
  }
  
  template < typename funT, typename invFunT >
  analyticFunctionCombination< funT, invFunT >
    ::analyticFunctionCombination(
      funT const * const fun, invFunT const * const invFun
  ) : funT(*fun) {
    _fun.reset( fun->clone() );
    _invFun.reset( invFun->clone() );   
  }
  
  template < typename funT, typename invFunT >
  analyticFunctionCombination < funT, invFunT > * 
  analyticFunctionCombination< funT, invFunT >::clone( 
    void 
  ) const {
    return new analyticFunctionCombination< funT, invFunT >( *this );    
  }

  template < typename funT, typename invFunT >  
	analyticFunctionCombination< funT, invFunT > * 
  analyticFunctionCombination< funT, invFunT >::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return 
      new analyticFunctionTransformed< 
        analyticFunctionCombination< funT, invFunT > 
      >(*this, dtT);
	}   
  
  template < typename funT, typename invFunT >
  analyticFunctionCombination < funT, invFunT > * 
  analyticFunctionCombination< funT, invFunT >::create( void ) const {
    return new analyticFunctionCombination< funT, invFunT >();
  }
  
  template < typename funT, typename invFunT >
  analyticFunctionCombination< funT, invFunT >
    ::~analyticFunctionCombination() {
    
  }
  
  template < typename funT, typename invFunT >
  aFY analyticFunctionCombination< funT, invFunT >
    ::Y( aFX const & xx ) const {
    return _fun->Y( xx );    
  }
  
  template < typename funT, typename invFunT >
  aFX analyticFunctionCombination< funT, invFunT >
    ::invY( aFY const & yy ) const {
    return _invFun->Y( analyticFunction::aFX_aFY(yy) );    
  }

  template < typename funT, typename invFunT >
  bool analyticFunctionCombination< funT, invFunT >
    ::closed( int const & dir ) const {
    return _fun->closed( dir );    
  }  
}
#endif	/* analyticFunctionCombination_H */