#ifndef map3dTo3dTransformed_H
#define	map3dTo3dTransformed_H

#include <dtTransformerHeaven/dtStrongTransformer.h>
#include "analyticGeometry.h"
#include "map3dTo3d.h"
#include <logMe/dtMacros.h>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace dtOO {
  template < typename funT >
  class map3dTo3dTransformed : public funT {
    BOOST_STATIC_ASSERT( (boost::is_base_of<map3dTo3d, funT>::value) );    
  public:
    dt__class(map3dTo3dTransformed, analyticGeometry); 
    map3dTo3dTransformed();
    map3dTo3dTransformed( map3dTo3dTransformed const & orig ); 
    map3dTo3dTransformed(funT const & orig );    
    map3dTo3dTransformed(funT const & orig, dtTransformer const * const dtT);
    virtual ~map3dTo3dTransformed();
    virtual map3dTo3dTransformed * clone(void) const;
    virtual map3dTo3dTransformed * cloneTransformed(
      dtTransformer const * const dtT
    ) const;
    virtual map3dTo3dTransformed * create( void ) const;
    virtual dtPoint3 getPoint( 
      float const & uu, float const & vv, float const & ww     
    ) const; 
    //
    //
    //
    virtual std::vector< dtVector3 > firstDer( 
      float const & uu, float const & vv, float const & ww 
    ) const;
    virtual dtPoint3 reparamInVolume(dtPoint3 const & ppXYZ) const;  
    virtual bool isTransformed( void ) const;    
  private:
    dt__pH(dtStrongTransformer) _dtT;
  };  
  
  template < typename funT >
  map3dTo3dTransformed< funT >::map3dTo3dTransformed() : funT() {
    
  }

  template < typename funT >  
  map3dTo3dTransformed< funT >::map3dTo3dTransformed(
    map3dTo3dTransformed const & orig
  ) : funT(orig) {
    _dtT.reset( orig._dtT->clone() );
  }  

  template < typename funT >  
  map3dTo3dTransformed< funT >::map3dTo3dTransformed(
    funT const & orig
  ) : funT(orig) {

  }    
  
  template < typename funT >
  map3dTo3dTransformed< funT >::map3dTo3dTransformed(
    funT const & orig, dtTransformer const * const dtT
  ) : funT(orig) {
    dtStrongTransformer const * const dtsT
    =
    dynamic_cast< dtStrongTransformer const * >(dtT);
    dt__throwIf(!dtsT, map3dTo3dTransformed());
    
    _dtT.reset( dtsT->clone() );
  }
  
  template < typename funT >  
  map3dTo3dTransformed< funT >::~map3dTo3dTransformed() {
    
  }
  
  template < typename funT >  
  map3dTo3dTransformed< funT > * map3dTo3dTransformed< funT >::clone( 
    void 
  ) const {
    return new map3dTo3dTransformed< funT >(*this);
  }
  
  template < typename funT >  
  map3dTo3dTransformed< funT > * map3dTo3dTransformed< funT >::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new map3dTo3dTransformed< funT >(*this, dtT);
  }
  
  template < typename funT >  
  map3dTo3dTransformed< funT > * map3dTo3dTransformed< funT >::create( 
    void 
  ) const {
    return new map3dTo3dTransformed< funT >();
  }
    
  template < typename funT >    
  dtPoint3 map3dTo3dTransformed< funT >::getPoint( 
    float const & uu, float const & vv, float const & ww 
  ) const {
    return _dtT->apply( funT::getPoint(uu, vv, ww) );
  }
  
  template < typename funT >
  std::vector< dtVector3 > map3dTo3dTransformed< funT >::firstDer( 
    float const & uu, float const & vv, float const & ww 
  ) const {
    return _dtT->apply( funT::firstDer(uu, vv, ww) );    
  }
   
  template < typename funT >    
  dtPoint3 map3dTo3dTransformed< funT >::reparamInVolume(
    dtPoint3 const & ppXYZ
  ) const {
    return map3dTo3d::reparamInVolume( ppXYZ );    
  }

  template < typename funT >
  bool map3dTo3dTransformed< funT >::isTransformed( void ) const {
    return true;
  }  
}
#endif	/* map3dTo3dTransformed_H */

