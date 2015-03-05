#ifndef ANALYTICFUNCTIONTRANSFORMED_H
#define	ANALYTICFUNCTIONTRANSFORMED_H

#include <progHelper.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include "analyticFunction.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  template < typename funT >
  class analyticFunctionTransformed : public funT {
  public:
    dt__CLASSSTD(analyticFunctionTransformed, analyticFunction); 
    analyticFunctionTransformed();
    analyticFunctionTransformed( analyticFunctionTransformed const & orig);      
    analyticFunctionTransformed(funT const & orig );
    virtual ~analyticFunctionTransformed();
    virtual analyticFunctionTransformed * clone( void ) const;
    virtual analyticFunctionTransformed * create( void ) const;
    virtual aFY Y( aFX const & xx ) const;
    void setTransformer( dtTransformer const * const dtT );
  private:
    dt__pH(dtTransformer) _dtT;
  };  
  
  template < typename funT >
  analyticFunctionTransformed< funT >::analyticFunctionTransformed() : funT() {
    
  }
  
  template < typename funT >  
  analyticFunctionTransformed< funT >::analyticFunctionTransformed(analyticFunctionTransformed const & orig) : funT(orig) {
    _dtT.reset( orig._dtT->clone() );
  }  
  
  template < typename funT >  
  analyticFunctionTransformed< funT >::analyticFunctionTransformed(funT const & orig) : funT(orig) {
    
  }  
  
  template < typename funT >  
  analyticFunctionTransformed< funT >::~analyticFunctionTransformed() {
    
  }
  
  template < typename funT >  
  analyticFunctionTransformed< funT > * analyticFunctionTransformed< funT >::clone( void ) const {
    return new analyticFunctionTransformed< funT >(*this);
  }
  
  template < typename funT >  
  analyticFunctionTransformed< funT > * analyticFunctionTransformed< funT >::create( void ) const {
    return new analyticFunctionTransformed< funT >();
  }

  template < typename funT >    
  aFY analyticFunctionTransformed< funT >::Y( aFX const & xx ) const {
    aFY yy = funT::Y(xx);
    
//    dt__THROW_IF( yy.size() != 3, Y() );
    if (yy.size() == 3) {
      dtPoint3 pp = _dtT->apply( dtPoint3(yy[0], yy[1], yy[2]) );
      yy[0] = pp.x();
      yy[1] = pp.y();
      yy[2] = pp.z();
    }
    else if(yy.size() == 2) {
      dtPoint2 pp = _dtT->apply( dtPoint2(yy[0], yy[1]) );
      yy[0] = pp.x();
      yy[1] = pp.y();
    }
    else {
      dt__THROW(Y(),
        << DTLOGEVAL(yy.size()) << LOGDEL
        << "Currently only supported for dimension 2 and 3."
      );
    }
    
    return yy;
  }
  
  template < typename funT >  
  void analyticFunctionTransformed< funT >::setTransformer( 
    dtTransformer const * const dtT 
  ) {
    _dtT.reset( dtT->clone() );
  }
}
#endif	/* ANALYTICFUNCTIONTRANSFORMED_H */

