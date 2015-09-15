#include "pickMap3dTo3dRangePercent.h"
#include "analyticGeometryHeaven/map2dTo3d.h"
#include "pickLengthPercentRange.h"
//#include "analyticGeometryHeaven/map3dTo3d.h"
#include <logMe/logMe.h>
#include <progHelper.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map3dTo3d.h>

namespace dtOO {
  pickMap3dTo3dRangePercent::pickMap3dTo3dRangePercent() : dtTransformer() {
  }

  pickMap3dTo3dRangePercent::pickMap3dTo3dRangePercent(
    const pickMap3dTo3dRangePercent& orig
  ) : dtTransformer(orig) {
		_u0 = orig._u0;
		_v0 = orig._v0;
		_w0 = orig._w0;
  }

  pickMap3dTo3dRangePercent::~pickMap3dTo3dRangePercent() {
  }

  dtTransformer * pickMap3dTo3dRangePercent::clone( void ) const {
	  return new pickMap3dTo3dRangePercent(*this);
	}
	
  dtTransformer * pickMap3dTo3dRangePercent::create( void ) const {
		return new pickMap3dTo3dRangePercent();
	}
	
  vectorHandling< analyticGeometry * > pickMap3dTo3dRangePercent::apply( 
    vectorHandling< analyticGeometry * > const * const aGeoVecP 
  ) const {
    vectorHandling< analyticGeometry * > aGeoRet;
    
    for (int ii=0;ii<aGeoVecP->size();ii++) {
      dt__ptrAss(map3dTo3d const * m3d, map3dTo3d::DownCast(aGeoVecP->at(ii)));
			
			analyticGeometry * aG;
			if (_u0 >= 0.) {
				aG = m3d->segmentConstUPercent(_u0);				
		  }
			else if (_v0 >= 0.) {
				aG = m3d->segmentConstVPercent(_v0);				
		  }
			else if (_w0 >= 0.) {
				aG = m3d->segmentConstWPercent(_w0);				
		  }		
      aGeoRet.push_back( aG );
    }
    
    return aGeoRet;
  }

  bool pickMap3dTo3dRangePercent::isNecessary( void ) const {
    return true;
  }
  
  void pickMap3dTo3dRangePercent::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    
		_u0 = -1.;
		_v0 = -1.;
		_w0 = -1.;

		if (dtXmlParserBase::hasAttribute("parameter_percent_one", *tE)) {
			_u0 
			= 
			dtXmlParserBase::muParseString( 
				dtXmlParserBase::replaceDependencies(
					dtXmlParserBase::getAttributeStr("parameter_percent_one", *tE),
					cV, 
					aF
				) 
			);
		}
		if (dtXmlParserBase::hasAttribute("parameter_percent_two", *tE)) {
			_v0
			= 
			dtXmlParserBase::muParseString( 
				dtXmlParserBase::replaceDependencies(
					dtXmlParserBase::getAttributeStr("parameter_percent_two", *tE),
					cV, 
					aF
				) 
			);
		}
		if (dtXmlParserBase::hasAttribute("parameter_percent_three", *tE)) {
			_w0
			= 
			dtXmlParserBase::muParseString( 
				dtXmlParserBase::replaceDependencies(
					dtXmlParserBase::getAttributeStr("parameter_percent_three", *tE),
					cV, 
					aF
				) 
			);							
		}
		
    if ( (_u0 >= 0.) && (_v0 >= 0.) && (_w0 >= 0.) ) {
			dt__throw(
      init(),
      << dt__eval(_u0) << std::endl
      << dt__eval(_v0) << std::endl
				<< dt__eval(_w0) << std::endl
				<< "Only one value should be greater than zero. Check input."
			);
    }
  }  
}
