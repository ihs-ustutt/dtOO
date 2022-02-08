#ifndef vec3dMultiThreeD_H
#define	vec3dMultiThreeD_H

#include <dtOOTypeDef.h>

#include <logMe/logMe.h>
#include <logMe/logContainer.h>
#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include "vec3dFunction.h"
#include "analyticFunctionTransformed.h"
#include <RTree.h>
#include <boost/assign.hpp>

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
      virtual void dump( void ) const;      
      static bool rTreeCallback(funT * aFun, void *ctx);
    private:
      funT const & findF_x( aFX const & xx ) const;
    private:
      std::map< std::pair< aFX, aFX >, funT * > _ff;
      RTree< funT *, dtReal, 3, dtReal > _x_rTree;
      RTree< funT *, dtReal, 3, dtReal > _y_rTree;
      constexpr static dtReal _tol = 1.e-8;
  };
  
  template < typename funT >  
  bool vec3dMultiThreeD< funT >::rTreeCallback(funT * aFun, void * ctx) {
    std::vector< funT * > * out = static_cast< std::vector< funT * > * >(ctx);
    out->push_back( aFun );
    return true; // we're done searching
  }  
  
  template < typename funT >  
  vec3dMultiThreeD< funT >::vec3dMultiThreeD() : funT() {
  }

  template < typename funT >      
  vec3dMultiThreeD< funT >::vec3dMultiThreeD( 
    vec3dMultiThreeD const & orig
  ) : funT(orig) {   
    //
    // copy functions
    //
    dt__forAllRefAuto(orig._ff, aPair) add( aPair.second->clone() );
  }
    
  template < typename funT >      
  vec3dMultiThreeD< funT >::~vec3dMultiThreeD() {
    dt__forAllRefAuto(_ff, aPair) delete aPair.second;
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
    aFX glMinX = funT::xMin();
    aFX glMaxX = funT::xMax();
    if (
      (glMinX[0]>xx[0]) || (glMinX[1]>xx[1]) || (glMinX[2]>xx[2]) 
      ||
      (glMaxX[0]<xx[0]) || (glMaxX[1]<xx[1]) || (glMaxX[2]<xx[2])
    ) {
      dt__throw(
        findF_x(),
        << "Cannot determine range " << dt__eval(xx) << std::endl
        << "Out of global range."
      );            
    }    

    std::vector< funT * > out;
    std::vector< dtReal > min
    = 
    ::boost::assign::list_of(xx[0]-_tol)(xx[1]-_tol)(xx[2]-_tol);
    std::vector< dtReal > max
    = 
    ::boost::assign::list_of(xx[0]+_tol)(xx[1]+_tol)(xx[2]+_tol);
    dtInt nEnt 
    = 
    const_cast< vec3dMultiThreeD< funT > * >(this)->_x_rTree.Search( 
      &min[0], &max[0], vec3dMultiThreeD< funT >::rTreeCallback, &out 
    );

    if ( nEnt == 1 ) {
      return *out[0];
    }
    else if ( nEnt > 1 ) {
      dt__forFromToIndex(0, nEnt, ii) {
        aFX minX = out[ii]->xMin();
        aFX maxX = out[ii]->xMax();
        if (
          (minX[0]<=xx[0]) && (minX[1]<=xx[1]) && (minX[2]<=xx[2]) 
          &&
          (maxX[0]>=xx[0]) && (maxX[1]>=xx[1]) && (maxX[2]>=xx[2])
        ) {
          return *out[ii];
        }
      }
    }
    
    dt__throw(
      findF_x(),
      << "Cannot determine range " << dt__eval(xx) << std::endl
      << "nEnt = " << nEnt
    );          
  }
       
  template < typename funT >      
  void vec3dMultiThreeD< funT >::add( funT * aFun ) {
    aFX thisMin = aFun->xMin();
    aFX thisMax = aFun->xMax();
    std::pair< aFY, aFY > yBB = aFun->yBoundingBox();
    
    //
    // add function
    //
    _ff[ std::pair< aFX, aFX >( thisMin, thisMax ) ] = aFun;
    
    //
    // adjust parameter range
    //
    dt__forFromToIndex(0, 3, ii) {
      if ( funT::xMin(ii) > thisMin[ii] ) funT::setMin(ii, thisMin[ii]);
      if ( funT::xMax(ii) < thisMax[ii] ) funT::setMax(ii, thisMax[ii]);
    }
    
    //
    // insert in R-tree
    //
    std::vector< dtReal > min
    = 
    ::boost::assign::list_of
      (thisMin[0] - _tol)
      (thisMin[1] - _tol)
      (thisMin[2] - _tol);
    std::vector< dtReal > max 
    = 
    ::boost::assign::list_of
      (thisMax[0] + _tol)
      (thisMax[1] + _tol)
      (thisMax[2] + _tol);
    _x_rTree.Insert(&min[0], &max[0], aFun);
  }
  
  template < typename funT >    
  void vec3dMultiThreeD< funT >::dump( void ) const {
    logContainer< vec3dMultiThreeD< funT > > dt__debugContainer(logC);   
    dtInt cc = 0;

    aFX xMin;
    aFX xMax;
    aFY yMin;
    aFY yMax;
    
    dt__forAllRefAuto(_ff, aPair) {
      xMin = aPair.first.first;
      xMax = aPair.first.second;
      yMin = aPair.second->Y(aPair.first.first);
      yMax = aPair.second->Y(aPair.first.second);
      logC() 
        << logMe::dtFormat(
          "min(%5i) : F( [%12.6e %12.6e %12.6e] ) = [%12.6e %12.6e %12.6e]"
        ) 
          % cc 
          % xMin[0] % xMin[1] % xMin[2]
          % yMin[0] % yMin[1] % yMin[2]
        << std::endl
        << logMe::dtFormat(
          "max(%5i) : F( [%12.6e %12.6e %12.6e] ) = [%12.6e %12.6e %12.6e]"
        ) 
          % cc
          % xMax[0] % xMax[1] % xMax[2]
          % yMax[0] % yMax[1] % yMax[2]              
        << std::endl;
      cc++;
    }
    xMin = funT::xMin();
    xMax = funT::xMax();
    yMin = Y(xMin);
    yMax = Y(xMax);    
    logC() 
      << logMe::dtFormat(
        "min() : F( [%12.6e %12.6e %12.6e] ) = [%12.6e %12.6e %12.6e]"
      ) 
        % xMin[0] % xMin[1] % xMin[2]
        % yMin[0] % yMin[1] % yMin[2]
      << std::endl
      << logMe::dtFormat(
        "max() : F( [%12.6e %12.6e %12.6e] ) = [%12.6e %12.6e %12.6e]"
      ) 
        % xMax[0] % xMax[1] % xMax[2]
        % yMax[0] % yMax[1] % yMax[2];
  }  
}
#endif	/* vec3dMultiThreeD_H */

