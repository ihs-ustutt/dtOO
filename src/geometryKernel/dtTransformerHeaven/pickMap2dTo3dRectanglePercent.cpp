#include "pickMap2dTo3dRectanglePercent.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>

namespace dtOO {
  pickMap2dTo3dRectanglePercent::pickMap2dTo3dRectanglePercent() 
    : dtTransformer() {
  }

  pickMap2dTo3dRectanglePercent::pickMap2dTo3dRectanglePercent(
    const pickMap2dTo3dRectanglePercent& orig
  ) : dtTransformer(orig) {
		_p0 = orig._p0;
		_p1 = orig._p1;
  }

  pickMap2dTo3dRectanglePercent::~pickMap2dTo3dRectanglePercent() {
  }

  dtTransformer * pickMap2dTo3dRectanglePercent::clone( void ) const {
	  return new pickMap2dTo3dRectanglePercent(*this);
	}
	
  dtTransformer * pickMap2dTo3dRectanglePercent::create( void ) const {
		return new pickMap2dTo3dRectanglePercent();
	}
	
  aGPtrVec pickMap2dTo3dRectanglePercent::apply( 
    aGPtrVec const * const aGeoVecP 
  ) const {
    aGPtrVec aGeoRet;
    
    dt__forAllRefAuto( *aGeoVecP, aG) {
      aGeoRet.push_back( 
        map2dTo3d::MustConstDownCast( aG )->segmentRectanglePercent(_p0, _p1) 
      );
    }
    
    return aGeoRet;
  }

  bool pickMap2dTo3dRectanglePercent::isNecessary( void ) const {
    return true;
  }
  
  void pickMap2dTo3dRectanglePercent::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    
    dt__throwIf( !dtXmlParserBase::hasChild("Point_2", *tE), init() );

    std::vector< ::QDomElement > wE 
    = 
    dtXmlParserBase::getChildVector("Point_2", *tE);
    
    dt__throwIf( wE.size() != 2, init() );
    
    _p0 = dtXmlParserBase::createDtPoint2( &(wE[0]), bC, cV, aF, aG );
    _p1 = dtXmlParserBase::createDtPoint2( &(wE[1]), bC, cV, aF, aG );
  }  
}
