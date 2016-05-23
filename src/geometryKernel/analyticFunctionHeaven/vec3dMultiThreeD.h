#ifndef vec3dMultiThreeD_H
#define	vec3dMultiThreeD_H

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include "vec3dFunction.h"
#include "analyticFunctionTransformed.h"

namespace dtOO {
  class dtTransformer;
  
  template < typename funT >  
  class vec3dMultiThreeD : public funT {
    public:
      dt__class(vec3dMultiThreeD, analyticFunction); 
      vec3dMultiThreeD();
      vec3dMultiThreeD( vec3dMultiThreeD const & orig);  
      virtual ~vec3dMultiThreeD();
      virtual vec3dMultiThreeD * clone( void ) const;
      virtual vec3dMultiThreeD * create( void ) const;       
      virtual aFY Y( aFX const & xx ) const;
      virtual void add( funT * aFun );
    private:
      funT const & findF_x( aFX const & xx ) const;
    private:
      std::map< std::pair< aFX, aFX >, funT * > _ff;
  };
  
  template < typename funT >  
  vec3dMultiThreeD< funT >::vec3dMultiThreeD() : funT() {
    
  }

  template < typename funT >      
  vec3dMultiThreeD< funT >::vec3dMultiThreeD( 
    vec3dMultiThreeD const & orig
  ) : funT(orig) {
    _ff = orig._ff;          
  }
    
  template < typename funT >      
  vec3dMultiThreeD< funT >::~vec3dMultiThreeD() {
    
  }
    
  template < typename funT >      
  vec3dMultiThreeD< funT > * vec3dMultiThreeD< funT >::clone( void ) const {
    return new vec3dMultiThreeD< funT >(*this);    
  }
  
  template < typename funT >      
  vec3dMultiThreeD< funT > * vec3dMultiThreeD< funT >::create( void ) const {
    return new vec3dMultiThreeD< funT >();    
  }

  template < typename funT >      
  aFY vec3dMultiThreeD< funT >::Y(aFX const & xx) const {
    return findF_x(xx).Y(xx);
  }

  template < typename funT >
  funT const & vec3dMultiThreeD< funT >::findF_x( aFX const & xx ) const {
    auto iter 
    = 
    std::find_if(
      _ff.cbegin(), 
      _ff.cend(), 
      [=]( const std::pair< std::pair< aFX, aFX >, funT * >& fn ) {  
        return 
          xx[0]>=fn.first.first[0] && xx[0]<=fn.first.second[0]
          &&
          xx[1]>=fn.first.first[1] && xx[1]<=fn.first.second[1]
          &&
          xx[2]>=fn.first.first[2] && xx[2]<=fn.first.second[2]; 
      }
    );
    
    if( iter==_ff.end() ) { 
      dt__throw(
        findF_x(),
        << "Cannot determine range " << dt__eval(xx)
      );    
    }

    return *(iter->second);    
  }
      
  template < typename funT >      
  void vec3dMultiThreeD< funT >::add( funT * aFun ) {
    aFX thisMin = aFun->xMin();
    aFX thisMax = aFun->xMax();
    
    //
    // add function
    //
    _ff[ std::pair< aFX, aFX >( thisMin, thisMax ) ] = aFun;
    
    //
    // adjust parameter range
    //
    if (
      (funT::xMin(0) > thisMin[0])
      ||
      (funT::xMin(1) > thisMin[1])
      ||
      (funT::xMin(2) > thisMin[2])
    ) {
      funT::setMin(0, thisMin[0]);
      funT::setMin(1, thisMin[1]);
      funT::setMin(2, thisMin[2]);
    }
    if (
      (funT::xMax(0) < thisMax[0])
      ||
      (funT::xMax(1) < thisMax[1])
      ||
      (funT::xMax(2) < thisMax[2])
    ) {
      funT::setMax(0, thisMax[0]);
      funT::setMax(1, thisMax[1]);
      funT::setMax(2, thisMax[2]);
    }
  }
}
#endif	/* vec3dMultiThreeD_H */

