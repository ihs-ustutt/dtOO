/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef scaMultiOneD_H
#define	scaMultiOneD_H

#include <dtOOTypeDef.h>

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
      virtual dtReal YFloat(dtReal const & xx) const;
      virtual dtReal invYFloat(dtReal const & yy) const;
      virtual void add( funT const & aFun );
      virtual void dump( void ) const;
    private:
      funT const & findF_x( dtReal const & xx ) const;
      funT const & findF_y( dtReal const & yy ) const;
    private:
      std::map< std::pair< dtReal, dtReal >, funT * > _ff;
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
  dtReal scaMultiOneD< funT >::YFloat(dtReal const & xx) const {
    return findF_x(xx).YFloat(xx);
  }

  template < typename funT >      
  dtReal scaMultiOneD< funT >::invYFloat(dtReal const & yy) const {
    return findF_y(yy).invYFloat(yy);    
  }
  
  template < typename funT >
  funT const & scaMultiOneD< funT >::findF_x( dtReal const & xx ) const {
    auto iter 
    = 
    std::find_if(
      _ff.cbegin(), 
      _ff.cend(), 
      [=]( const std::pair< std::pair< dtReal, dtReal >, funT * >& fn ) {  
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
  funT const & scaMultiOneD< funT >::findF_y( dtReal const & yy ) const {
    auto iter 
    = 
    std::find_if(
      _ff.cbegin(), 
      _ff.cend(), 
      [=]( const std::pair< std::pair< dtReal, dtReal >, funT * >& fn ) {  
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
          const std::pair< std::pair< dtReal, dtReal >, funT * >& fnA, 
          const std::pair< std::pair< dtReal, dtReal >, funT * >& fnB 
        ) {  
          dtReal thisMinA 
          =
          std::min(
            fabs(yy-fnA.second->YFloat(fnA.first.first)),
            fabs(yy-fnA.second->YFloat(fnA.first.second))
          );
          dtReal thisMinB 
          =
          std::min(
            fabs(yy-fnB.second->YFloat(fnB.first.first)),
            fabs(yy-fnB.second->YFloat(fnB.first.second))
          );
          if (thisMinA < thisMinB) {
            return true;
          }
          else {
            return false;
          }
        }
      );    
    }
    
    return *(iter->second);
  }
      
  template < typename funT >      
  void scaMultiOneD< funT >::add( funT const & aFun ) {
    dtReal xMin = aFun.xMin(0);
    dtReal xMax = aFun.xMax(0);
    
    //
    // add function
    //
    _ff[ std::pair< dtReal, dtReal >( xMin, xMax ) ] 
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
