#ifndef ANALYTICFUNCTIONTRANSFORMED_H
#define	ANALYTICFUNCTIONTRANSFORMED_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtTransformerHeaven/dtTransformer.h>
#include "analyticFunction.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  template < typename funT >
  class analyticFunctionTransformed : public funT {
  public:
    dt__class(analyticFunctionTransformed, analyticFunction); 
    analyticFunctionTransformed();
    analyticFunctionTransformed( analyticFunctionTransformed const & orig);      
    analyticFunctionTransformed(funT const & orig );
    analyticFunctionTransformed(funT const & orig, dtTransformer const * const dtT);    
    virtual ~analyticFunctionTransformed();
    virtual analyticFunctionTransformed * clone( void ) const;
    virtual analyticFunctionTransformed * cloneTransformed(
      dtTransformer const * const dtT
    ) const;    
    virtual analyticFunctionTransformed * create( void ) const;
    virtual aFY Y( aFX const & xx ) const;
    virtual aFX invY( aFY const & yy ) const;
    void setTransformer( dtTransformer const * const dtT );
    virtual bool isTransformed( void ) const;
  private:
    dt__pH(dtTransformer) _dtT;
  };  
  
  template < typename funT >
  analyticFunctionTransformed< funT >::analyticFunctionTransformed() : funT() {
    
  }
  
  template < typename funT >  
  analyticFunctionTransformed< funT >::analyticFunctionTransformed(
    analyticFunctionTransformed const & orig
  ) : funT(orig) {
    _dtT.reset( orig._dtT->clone() );
  }  
  
  template < typename funT >  
  analyticFunctionTransformed< funT >::analyticFunctionTransformed(
    funT const & orig
  ) : funT(orig) {
    
  }  
  
  template < typename funT >
  analyticFunctionTransformed< funT >::analyticFunctionTransformed(
    funT const & orig, dtTransformer const * const dtT
  ) : funT(orig) {
    _dtT.reset( dtT->clone() );
  }  
  
  template < typename funT >  
  analyticFunctionTransformed< funT >::~analyticFunctionTransformed() {
    
  }
  
  template < typename funT >  
  analyticFunctionTransformed< funT > * 
  analyticFunctionTransformed< funT >::clone( void ) const {
    return new analyticFunctionTransformed< funT >(*this);
  }
  
  template < typename funT >  
  analyticFunctionTransformed< funT > * 
  analyticFunctionTransformed< funT >::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< funT >(*this, dtT);
  }
  
  template < typename funT >  
  analyticFunctionTransformed< funT > * 
  analyticFunctionTransformed< funT >::create( void ) const {
    return new analyticFunctionTransformed< funT >();
  }

  template < typename funT >    
  aFY analyticFunctionTransformed< funT >::Y( aFX const & xx ) const {
    aFY yy = funT::Y(xx);
    
//    dt__throwIf( yy.size() != 3, Y() );
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
      dt__throw(Y(),
        << dt__eval(yy.size()) << std::endl
        << "Currently only supported for dimension 2 and 3."
      );
    }
    
    return yy;
  }
  
  template < typename funT >    
  aFX analyticFunctionTransformed< funT >::invY( aFY const & yy ) const {
    aFY yyC = yy;    
    if (yyC.size() == 3) {
      dtPoint3 pp = _dtT->retract( dtPoint3(yyC[0], yyC[1], yyC[2]) );
      yyC[0] = pp.x();
      yyC[1] = pp.y();
      yyC[2] = pp.z();
    }
    else if(yyC.size() == 2) {
      dtPoint2 pp = _dtT->retract( dtPoint2(yyC[0], yyC[1]) );
      yyC[0] = pp.x();
      yyC[1] = pp.y();
    }
    else dt__throwUnexpected(Y());
    
    return funT::invY(yyC);
  }  
  
  template < typename funT >  
  void analyticFunctionTransformed< funT >::setTransformer( 
    dtTransformer const * const dtT 
  ) {
    _dtT.reset( dtT->clone() );
  }

  template < typename funT >    
	bool analyticFunctionTransformed< funT >::isTransformed( void ) const {
		return true;
	}  
}
#endif	/* ANALYTICFUNCTIONTRANSFORMED_H */

