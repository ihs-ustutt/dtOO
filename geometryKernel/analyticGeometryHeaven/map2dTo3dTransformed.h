#ifndef MAP2DTO3DTRANSFORMED_H
#define	MAP2DTO3DTRANSFORMED_H

#include <dtTransformerHeaven/dtTransformer.h>
#include "analyticGeometry.h"
#include "map2dTo3d.h"
#include <logMe/dtMacros.h>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace dtOO {
  template < typename funT >
  class map2dTo3dTransformed : public funT {
    BOOST_STATIC_ASSERT( (boost::is_base_of<map2dTo3d, funT>::value) );    
  public:
    dt__class(map2dTo3dTransformed, analyticGeometry); 
    map2dTo3dTransformed();
    map2dTo3dTransformed( map2dTo3dTransformed const & orig ); 
    map2dTo3dTransformed(funT const & orig );    
    map2dTo3dTransformed(funT const & orig, dtTransformer const * const dtT);
    virtual ~map2dTo3dTransformed();
    virtual map2dTo3dTransformed * clone( void ) const;
    virtual map2dTo3dTransformed * create( void ) const;
    virtual dtPoint3 getPoint( float const & uu, float const & vv ) const;
    void setTransformer( dtTransformer const * const dtT );    
    static void setTransformer( map2dTo3d * const orig, dtTransformer const * const dtT );    
  private:
    dt__pH(dtTransformer) _dtT;
  };  
  
  template < typename funT >
  map2dTo3dTransformed< funT >::map2dTo3dTransformed() : funT() {
    
  }

  template < typename funT >  
  map2dTo3dTransformed< funT >::map2dTo3dTransformed(map2dTo3dTransformed const & orig) : funT(orig) {
    _dtT.reset( orig._dtT->clone() );
  }  

  template < typename funT >  
  map2dTo3dTransformed< funT >::map2dTo3dTransformed(funT const & orig) : funT(orig) {
  }    
  
  template < typename funT >
  map2dTo3dTransformed< funT >::map2dTo3dTransformed(funT const & orig, dtTransformer const * const dtT) : funT(orig) {
    _dtT.reset( dtT->clone() );
  }
  
  template < typename funT >  
  map2dTo3dTransformed< funT >::~map2dTo3dTransformed() {
    
  }
  
  template < typename funT >  
  map2dTo3dTransformed< funT > * map2dTo3dTransformed< funT >::clone( void ) const {
    return new map2dTo3dTransformed< funT >(*this);
  }
  
  template < typename funT >  
  map2dTo3dTransformed< funT > * map2dTo3dTransformed< funT >::create( void ) const {
    return new map2dTo3dTransformed< funT >();
  }
    
  template < typename funT >    
  dtPoint3 map2dTo3dTransformed< funT >::getPoint( float const & uu, float const & vv ) const {
    dtPoint3 pp = funT::getPoint(uu, vv);
    
    return _dtT->apply(pp);
  }
  
  template < typename funT >  
  void map2dTo3dTransformed< funT >::setTransformer( 
    dtTransformer const * const dtT 
  ) {
    _dtT.reset( dtT->clone() );
  }  
  template < typename funT >
  void map2dTo3dTransformed< funT >::setTransformer( 
    map2dTo3d * const orig, dtTransformer const * const dtT 
  ) {
//    orig->setTransformer( dtT );
  }    
}
#endif	/* MAP2DTO3DTRANSFORMED_H */

