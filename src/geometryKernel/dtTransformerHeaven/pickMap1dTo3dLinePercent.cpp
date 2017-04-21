#include "pickMap1dTo3dLinePercent.h"
#include "analyticGeometryHeaven/map2dTo3d.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>

namespace dtOO {
  pickMap1dTo3dLinePercent::pickMap1dTo3dLinePercent() : dtTransformer() {
    _u0 = -1.;
		_u1 = -1.;
  }

  pickMap1dTo3dLinePercent
    ::pickMap1dTo3dLinePercent(const pickMap1dTo3dLinePercent& orig)
    : dtTransformer(orig) {
		_u0 = orig._u0;
		_u1 = orig._u1;
  }

  pickMap1dTo3dLinePercent::~pickMap1dTo3dLinePercent() {
  }

  dtTransformer * pickMap1dTo3dLinePercent::clone( void ) const {
	  return new pickMap1dTo3dLinePercent(*this);
	}
	
  dtTransformer * pickMap1dTo3dLinePercent::create( void ) const {
		return new pickMap1dTo3dLinePercent();
	}
	
  aGPtrVec pickMap1dTo3dLinePercent::apply( 
    aGPtrVec const * const aGeoVecP 
  ) const {
    aGPtrVec aGeoRet;
    
    dt__forAllRefAuto(*aGeoVecP, aG) {
      map1dTo3d const * m1d = map1dTo3d::MustConstDownCast( aG );
			
      aGeoRet.push_back( m1d->segmentPercent( _u0, _u1 ) );
    }
    
    return aGeoRet;
  }

  bool pickMap1dTo3dLinePercent::isNecessary( void ) const {
    return true;
  }
  
  void pickMap1dTo3dLinePercent::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);

    std::vector< ::QDomElement > el 
    = 
    dtXmlParserBase::getChildVector("float", *tE);
    dt__throwIf( el.size()!=2, init() );
    
    _u0 = dtXmlParserBase::createFloat(el[0], bC, cV, aF, aG);
    _u1 = dtXmlParserBase::createFloat(el[1], bC, cV, aF, aG);
  }  
}
