#include "pickMap2dTo3dRangePercent.h"
#include "analyticGeometryHeaven/map2dTo3d.h"
#include <logMe/logMe.h>
#include <progHelper.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>

namespace dtOO {
  pickMap2dTo3dRangePercent::pickMap2dTo3dRangePercent() : dtTransformer() {
  }

  pickMap2dTo3dRangePercent::pickMap2dTo3dRangePercent(const pickMap2dTo3dRangePercent& orig) : dtTransformer(orig) {
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
	
  vectorHandling< analyticGeometry * > pickMap2dTo3dRangePercent::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
    vectorHandling< analyticGeometry * > aGeoRet;
    
    for (int ii=0;ii<aGeoVecP->size();ii++) {
      dt__PTRASS(map2dTo3d const * m2d, map2dTo3d::DownCast(aGeoVecP->at(ii)));
			
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
	  QDomElement const * transformerElementP, 
    baseContainer const * const bC,  
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP 
	) {
    dtTransformer::init(transformerElementP, bC, cValP, sFunP, depAGeoP);
    
		_u0 = -1.;
		_v0 = -1.;

		if ( hasAttribute("parameter_percent_one", *transformerElementP) ) {
			_u0 
			= 
			muParseString( 
				replaceUsedFunctions(
					getAttributeStr("parameter_percent_one", *transformerElementP),
					cValP, 
					sFunP
				) 
			);
		}
		if ( hasAttribute("parameter_percent_two", *transformerElementP) ) {
			_v0
			= 
			muParseString( 
				replaceUsedFunctions(
					getAttributeStr("parameter_percent_two", *transformerElementP),
					cValP, 
					sFunP
				) 
			);
		}
		
    if ( (_u0 >= 0.) && (_v0 >= 0.) ) {
			dt__THROW(
				init(),
				<< DTLOGEVAL(_u0) << LOGDEL
				<< DTLOGEVAL(_v0) << LOGDEL
				<< "Only one value should be greater than zero. Check input."
			);
    }
  }  
}
