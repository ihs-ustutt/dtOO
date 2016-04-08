#ifndef scaMultiOneD_H
#define	scaMultiOneD_H

#include <dtLinearAlgebra.h>
#include <string>
#include <logMe/dtMacros.h>
#include "scaFunction.h"
#include "analyticFunctionTransformed.h"

namespace dtOO {
  class dtTransformer;
  
  template < typename funT >  
  class scaMultiOneD : public funT {
    public:
      dt__class(scaMultiOneD, analyticFunction); 
      scaMultiOneD();
      scaMultiOneD( scaMultiOneD const & orig);  
      virtual ~scaMultiOneD();
      virtual scaMultiOneD * clone( void ) const;
      virtual scaMultiOneD * create( void ) const;       
      virtual float YFloat(float const & xx) const;
      virtual float invYFloat(float const & yy) const;
      virtual void add( funT const & aFun );
      virtual void dump( void ) const;
    private:
      funT const & findF_x( float const & xx ) const;
      funT const & findF_y( float const & yy ) const;
    private:
      std::map< std::pair< float, float >, funT * > _ff;
  };
  
  template < typename funT >  
  scaMultiOneD< funT >::scaMultiOneD() : funT() {
    
  }

  template < typename funT >      
  scaMultiOneD< funT >::scaMultiOneD( scaMultiOneD const & orig) : funT(orig) {
    _ff = orig._ff;          
    funT::setMinMax(funT::xMin(0), funT::xMax(0));
  }
    
  template < typename funT >      
  scaMultiOneD< funT >::~scaMultiOneD() {
    
  }
    
  template < typename funT >      
  scaMultiOneD< funT > * scaMultiOneD< funT >::clone( void ) const {
    return new scaMultiOneD< funT >(*this);    
  }
  
  template < typename funT >      
  scaMultiOneD< funT > * scaMultiOneD< funT >::create( void ) const {
    return new scaMultiOneD< funT >();    
  }

  template < typename funT >      
  float scaMultiOneD< funT >::YFloat(float const & xx) const {
    return findF_x(xx).YFloat(xx);
  }

  template < typename funT >      
  float scaMultiOneD< funT >::invYFloat(float const & yy) const {
    return findF_y(yy).invYFloat(yy);    
  }
  
  template < typename funT >
  funT const & scaMultiOneD< funT >::findF_x( float const & xx ) const {
    auto iter 
    = 
    std::find_if(
      _ff.cbegin(), 
      _ff.cend(), 
      [=]( const std::pair< std::pair< float, float >, funT * >& fn ) {  
        return xx>=fn.first.first &&  xx<=fn.first.second; 
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
  funT const & scaMultiOneD< funT >::findF_y( float const & yy ) const {
    auto iter 
    = 
    std::find_if(
      _ff.cbegin(), 
      _ff.cend(), 
      [=]( const std::pair< std::pair< float, float >, funT * >& fn ) {  
        return 
          yy>=fn.second->YFloat(fn.first.first) 
          && 
          yy<=fn.second->YFloat(fn.first.second);
      }
    );
    
    if( iter==_ff.end() ) { 
      dt__debug(
        findF_y(),
        << "Cannot determine range " << dt__eval(yy) << std::endl
        << "Find best fit."
      );    
      
      dump();
    
      //
      // find minimum
      //
      iter 
      =       
      std::min_element(
        _ff.cbegin(), 
        _ff.cend(), 
        [=]( 
          const std::pair< std::pair< float, float >, funT * >& fnA, 
          const std::pair< std::pair< float, float >, funT * >& fnB 
        ) {  
          return 
            std::min(
              std::min(
                fabs(yy-fnA.second->YFloat(fnA.first.first)),
                fabs(yy-fnA.second->YFloat(fnA.first.second))
              ),
              std::min(
                fabs(yy-fnB.second->YFloat(fnB.first.first)),
                fabs(yy-fnB.second->YFloat(fnB.first.second))
              )
            );
        }
      );    
    }
    
    return *(iter->second);
  }
      
  template < typename funT >      
  void scaMultiOneD< funT >::add( funT const & aFun ) {
    float xMin = aFun.xMin(0);
    float xMax = aFun.xMax(0);
    
    //
    // add function
    //
    _ff[ std::pair< float, float >( xMin, xMax ) ] 
    = 
    aFun.clone();
    
    //
    // adjust parameter range
    //
    if (funT::xMin(0) > xMin) funT::setMin(xMin);
    if (funT::xMax(0) < xMax) funT::setMax(xMax);
  }  

  template < typename funT >    
  void scaMultiOneD< funT >::dump( void ) const {
    logContainer< scaMultiOneD< funT > > logC(TLogLevel::logDEBUG, "dump()");    
    dt__forAllRefAuto(_ff, aPair) {
      logC() 
        << logMe::dtFormat("F( [%12.6e %12.6e] ) = [%12.6e %12.6e]") 
          % aPair.first.first 
          % aPair.first.second 
          % aPair.second->YFloat(aPair.first.first) 
          % aPair.second->YFloat(aPair.first.second) 
        << std::endl;
    }
  }
}
#endif	/* scaMultiOneD_H */

