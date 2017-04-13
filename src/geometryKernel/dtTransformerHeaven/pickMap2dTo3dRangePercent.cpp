#include "pickMap2dTo3dRangePercent.h"
#include "analyticGeometryHeaven/map2dTo3d.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>

namespace dtOO {
  pickMap2dTo3dRangePercent::pickMap2dTo3dRangePercent() : dtTransformer() {
  }

  pickMap2dTo3dRangePercent
    ::pickMap2dTo3dRangePercent(const pickMap2dTo3dRangePercent& orig)
    : dtTransformer(orig) {
		_u0 = orig._u0;
		_v0 = orig._v0;
  }

  pickMap2dTo3dRangePercent::~pickMap2dTo3dRangePercent() {
  }

  dtTransformer * pickMap2dTo3dRangePercent::clone( void ) const {
	  return new pickMap2dTo3dRangePercent(*this);
	}
	
  dtTransformer * pickMap2dTo3dRangePercent::create( void ) const {
		return new pickMap2dTo3dRangePercent();
	}
	
  aGPtrVec pickMap2dTo3dRangePercent::apply( 
    aGPtrVec const * const aGeoVecP 
  ) const {
    aGPtrVec aGeoRet;
    
    for (int ii=0;ii<aGeoVecP->size();ii++) {
      dt__ptrAss(map2dTo3d const * m2d, map2dTo3d::DownCast(aGeoVecP->at(ii)));
			
			map1dTo3d * m1d;
			if (_u0 >= 0.) {
				m1d = m2d->segmentConstUPercent(_u0);				
		  }
			else if (_v0 >= 0.) {
				m1d = m2d->segmentConstVPercent(_v0);				
		  }
      aGeoRet.push_back( m1d );
    }
    
    return aGeoRet;
  }

  bool pickMap2dTo3dRangePercent::isNecessary( void ) const {
    return true;
  }
  
  void pickMap2dTo3dRangePercent::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    
		_u0 = -1.;
		_v0 = -1.;

		if ( dtXmlParserBase::hasAttribute("parameter_percent_one", *tE) ) {
			_u0 
			= 
			dtXmlParserBase::getAttributeFloatMuParse( 
        "parameter_percent_one", *tE, cV, aF, aG
			);
		}
		if ( dtXmlParserBase::hasAttribute("parameter_percent_two", *tE) ) {
			_v0
			= 
			dtXmlParserBase::getAttributeFloatMuParse( 
        "parameter_percent_two", *tE, cV, aF, aG
			);
		}
		
    dt__throwIf( (_u0 >= 0.) && (_v0 >= 0.), init() );
  }  
}
