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

#ifndef map2dTo3dTransformed_H
#define	map2dTo3dTransformed_H

#include <dtOOTypeDef.h>

#include <dtTransformerHeaven/dtTransformerInvThreeD.h>
#include "analyticGeometry.h"
#include "map2dTo3d.h"
#include <logMe/dtMacros.h>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <discrete3dPoints.h>

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
    virtual map2dTo3dTransformed * clone(void) const;
    virtual map2dTo3dTransformed * cloneTransformed(
      dtTransformer const * const dtT
    ) const;
    virtual map2dTo3dTransformed * create( void ) const;
    virtual dtPoint3 getPoint( dtReal const & uu, dtReal const & vv ) const; 
    virtual vectorHandling< renderInterface * > getExtRender( void ) const;      
    //
    //
    //
    virtual dtVector3 normal( dtReal const & uu, dtReal const & vv) const;
    virtual std::vector< dtVector3 > firstDer( 
      dtReal const & uu, dtReal const & vv
    ) const;
    virtual std::vector< dtVector3 > secondDer( 
      dtReal const & uu, dtReal const & vv
    ) const;    
    virtual dtPoint2 reparamOnFace(dtPoint3 const & ppXYZ) const;  
    virtual bool isTransformed( void ) const;
  private:
    dt__pH(dtTransformerInvThreeD) _dtT;
  };  
  
  template < typename funT >
  map2dTo3dTransformed< funT >::map2dTo3dTransformed() : funT() {
    
  }

  template < typename funT >  
  map2dTo3dTransformed< funT >::map2dTo3dTransformed(
    map2dTo3dTransformed const & orig
  ) : funT(orig) {
    _dtT.reset( orig._dtT->clone() );
  }  

  template < typename funT >  
  map2dTo3dTransformed< funT >::map2dTo3dTransformed(
    funT const & orig
  ) : funT(orig) {

  }    
  
  template < typename funT >
  map2dTo3dTransformed< funT >::map2dTo3dTransformed(
    funT const & orig, dtTransformer const * const dtT
  ) : funT(orig) {
    dtTransformerInvThreeD const * const dtsT
    =
    dynamic_cast< dtTransformerInvThreeD const * >(dtT);
    dt__throwIf(!dtsT, map2dTo3dTransformed());
    
    _dtT.reset( dtsT->clone() );
  }
  
  template < typename funT >  
  map2dTo3dTransformed< funT >::~map2dTo3dTransformed() {
    
  }
  
  template < typename funT >  
  map2dTo3dTransformed< funT > * map2dTo3dTransformed< funT >::clone( 
    void 
  ) const {
    return new map2dTo3dTransformed< funT >(*this);
  }
  
  template < typename funT >  
  map2dTo3dTransformed< funT > * map2dTo3dTransformed< funT >::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new map2dTo3dTransformed< funT >(*this, dtT);
  }
  
  template < typename funT >  
  map2dTo3dTransformed< funT > * map2dTo3dTransformed< funT >::create( 
    void 
  ) const {
    return new map2dTo3dTransformed< funT >();
  }
    
  template < typename funT >    
  dtPoint3 map2dTo3dTransformed< funT >::getPoint( 
    dtReal const & uu, dtReal const & vv 
  ) const {
    dtPoint3 pp = funT::getPoint(uu, vv);
    
    return _dtT->apply(pp);
  }
  
  template < typename funT >
  vectorHandling< renderInterface * > 
  map2dTo3dTransformed< funT >::getExtRender( void ) const {
    vectorHandling< renderInterface * > rI = funT::getExtRender();
    
    dt__forAllRefAuto(rI, aRI) {
      if ( discrete3dPoints::Is( aRI ) ) {
        discrete3dPoints::SecureCast( aRI )->refP3()
        = 
        _dtT->apply( discrete3dPoints::SecureCast( aRI )->constRefP3() );
      }
    }
    return rI;
  }  
  
  template < typename funT >
  dtVector3 map2dTo3dTransformed< funT >::normal( 
    dtReal const & uu, dtReal const & vv
  ) const {
    return _dtT->apply( funT::normal(uu, vv) );
  }
  
  template < typename funT >
  std::vector< dtVector3 > map2dTo3dTransformed< funT >::firstDer( 
    dtReal const & uu, dtReal const & vv
  ) const {
    return _dtT->apply( funT::firstDer(uu, vv) );    
  }
  
  template < typename funT >
  std::vector< dtVector3 > map2dTo3dTransformed< funT >::secondDer( 
    dtReal const & uu, dtReal const & vv
  ) const {
    return _dtT->apply( funT::secondDer(uu, vv) );   
  }
  
  template < typename funT >    
  dtPoint2 map2dTo3dTransformed< funT >::reparamOnFace(
    dtPoint3 const & ppXYZ
  ) const {
    return map2dTo3d::reparamOnFace( ppXYZ );    
  }
  
  template < typename funT >
  bool map2dTo3dTransformed< funT >::isTransformed( void ) const {
    return true;
  }
}
#endif	/* map2dTo3dTransformed_H */
