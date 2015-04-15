#include "pickVec3dTwoDRangePercent.h"
#include <logMe/logMe.h>
#include <progHelper.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>

namespace dtOO {
  pickVec3dTwoDRangePercent::pickVec3dTwoDRangePercent() : dtTransformer() {
  }

  pickVec3dTwoDRangePercent::pickVec3dTwoDRangePercent(
	  const pickVec3dTwoDRangePercent& orig
	) : dtTransformer(orig) {
		_x0 = orig._x0;
		_x1 = orig._x1;
  }

  pickVec3dTwoDRangePercent::~pickVec3dTwoDRangePercent() {
  }

  dtTransformer * pickVec3dTwoDRangePercent::clone( void ) const {
	  return new pickVec3dTwoDRangePercent(*this);
	}
	
  dtTransformer * pickVec3dTwoDRangePercent::create( void ) const {
		return new pickVec3dTwoDRangePercent();
	}
	
  vectorHandling< analyticFunction * > pickVec3dTwoDRangePercent::apply( 
	  vectorHandling< analyticFunction * > const * const aFVecP 
	) const {
    vectorHandling< analyticFunction * > ret;
    
    for (int ii=0;ii<aFVecP->size();ii++) {
      dt__ptrAss(
				vec3dTwoD const * const v3d, 
				vec3dTwoD::ConstDownCast(aFVecP->at(ii))
			);

			vec3dSurfaceTwoD const * const v3dSurface 
			= 
			vec3dSurfaceTwoD::ConstDownCast(v3d);
			
			if (v3dSurface) {
				dt__pH(dtCurve) dtC;
				if (_x0 >= 0.) {
					dtC.reset( v3dSurface->ptrDtSurface()->segmentConstUPercent(_x0) );
				}
				else if (_x1 >= 0.) {
					dtC.reset( v3dSurface->ptrDtSurface()->segmentConstVPercent(_x1) );
				}
				ret.push_back( new vec3dCurveOneD( dtC.get() ) );
			}
			else {
				dt__throw(
					apply(), 
					<< "Incompatible type." << std::endl
					<< dt__eval(v3d) << std::endl
					<< dt__eval(v3dSurface)
				);		
			}
    }
		
		return ret;
	}

  bool pickVec3dTwoDRangePercent::isNecessary( void ) const {
    return true;
  }
  
  void pickVec3dTwoDRangePercent::init( 
	  QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    
		_x0 = -1.;
		_x1 = -1.;

		if ( dtXmlParserBase::hasAttribute("x_one_percent", *tE) ) {
			_x0 
			= 
			dtXmlParserBase::muParseString( 
				dtXmlParserBase::replaceUsedFunctions(
					dtXmlParserBase::getAttributeStr("x_one_percent", *tE), cV, aF
				) 
			);
		}
		if ( dtXmlParserBase::hasAttribute("x_two_percent", *tE) ) {
			_x1
			= 
			dtXmlParserBase::muParseString( 
				dtXmlParserBase::replaceUsedFunctions(
					dtXmlParserBase::getAttributeStr("x_two_percent", *tE), cV, aF
				) 
			);
		}
		
    dt__throwIf( (_x0 >= 0.) && (_x1 >= 0.), init() );
  }  
}
