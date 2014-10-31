#ifndef TRANSINT_H
#define	TRANSINT_H

//#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/vectorHandling.h>
#include <geometryEngine/dtCurve.h>

namespace dtOO {
  class dtCurve;
  
  template < typename T >
  class transInt {
  public:
    DTCLASSNAMEMETHOD(transInt);
    friend DTCLASSLOGMETHODI(transInt< T >,
            << "No member attributes.");
    transInt();
    transInt(std::vector< dtCurve * > const & boundary);
//    transInt(const transInt& orig);
    virtual ~transInt();
    T getValue(float const par1, float const par2) const;
    T getValue(T const p01, T const p12, T const p23, T const p03, 
               T const c1, T const c2, T const c3, T const c0, 
               float const par1, float const par2) const;
  private:
    vectorHandling< dtCurve * > _curve;
    T _p01;
    T _p03;
    T _p23;
    T _p12;    
  };
  
  template < typename T >
  transInt< T >::transInt() {
  }

//  template < typename T >  
//  transInt< T >::transInt(const transInt& orig) {
//  }

  template < typename T >
  transInt< T >::~transInt() {
  }
  
  template < typename T >
  transInt< T >::transInt(std::vector< dtCurve * > const & boundary) {
    if (boundary.size() != 4) {
      dt__THROW(transInt(),
              << "Wrong number of curves." << LOGDEL
              << DTLOGEVAL(boundary.size()) );
    }
    dt__FORALL(boundary, ii,
      _curve.push_back( boundary[ii] );
    );
    
    _curve[0]->getPointPercent(0, _p01);
    _curve[0]->getPointPercent(1, _p03);
    _curve[2]->getPointPercent(1, _p23);
    _curve[2]->getPointPercent(0, _p12);    
  }
  
  template< typename T >
  T transInt< T >::getValue(float const par1, float const par2) const {
    T c0;
    T c1;
    T c2;
    T c3;
    _curve[0]->getPointPercent(par1, c0);
    _curve[2]->getPointPercent(par1, c2);
    _curve[1]->getPointPercent(par2, c1);
    _curve[3]->getPointPercent(par2, c3);
    
    return getValue(_p01, _p12, _p23, _p03, c1, c2, c3, c0, par1, par2);   
  }

  template< typename T >
  T transInt< T >::getValue(T const p01, T const p12, T const p23, T const p03, 
                            T const c1, T const c2, T const c3, T const c0, 
                            float const par1, float const par2) const {
    T pUV = (1.-par1)*c1 + par1*c3 + (1.-par2)*c0 + par2*c2
            - ( 
                (1.-par1)*(1.-par2)*p01 + par1*par2*p23
                 + par1*(1.-par2)*p03 + (1.-par1)*par2*p12
              );   
    return pUV;
  }  
}
#endif	/* TRANSINT_H */

