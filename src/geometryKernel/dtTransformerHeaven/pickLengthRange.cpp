#include "pickLengthRange.h"
#include "geometryEngine/dtCurve.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <geometryEngine/dtSurface.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dOneD.h>

namespace dtOO {
  pickLengthRange::pickLengthRange() : dtTransformer() {
  }

  pickLengthRange::pickLengthRange(const pickLengthRange& orig) : dtTransformer(orig) {
  }

  pickLengthRange::~pickLengthRange() {
  }

  dtTransformer * pickLengthRange::clone( void ) const {
	  dt__throw(clone(), << "Not yet implemented.");
	}
	
  dtTransformer * pickLengthRange::create( void ) const {
		return new pickLengthRange();
	}
	
  aGPtrVec pickLengthRange::apply( aGPtrVec const * const aGeoVecP ) const {
    aGPtrVec aGeoRet;
    
    for (int ii=0;ii<aGeoVecP->size();ii++) {
      analyticGeometry * aGP = aGeoVecP->at(ii)->clone();
      //
      // cast
      //
      analyticSurface * aSP;
      dt__mustCast(aGP, analyticSurface, aSP);

      //
      if ( _createSplineCurve3d ) {
				dt__throw(apply(), << "Not yet supported.");
      }
      else if (_createAnalyticSurface) {
				dt__pH(map1dTo3d) m1dU(aSP->segmentConstVPercent(0., 0., 1.));
				dt__ptrAss(splineCurve3d * s3dU, splineCurve3d::DownCast(m1dU.get()));
				float u0 = s3dU->ptrConstDtCurve()->uPercent_l(_lengthOneStart);
				float u1 = s3dU->ptrConstDtCurve()->uPercent_l(_lengthOneEnd);
				dt__pH(map1dTo3d) m1dV(aSP->segmentConstUPercent(0., 0., 1.));
				dt__ptrAss(splineCurve3d * s3dV, splineCurve3d::DownCast(m1dV.get()));
				float v0 = s3dV->ptrConstDtCurve()->uPercent_l(_lengthTwoStart);
				float v1 = s3dV->ptrConstDtCurve()->uPercent_l(_lengthTwoEnd);
				
        map2dTo3d * m2d 
				= 
				aSP->segmentRectanglePercent(dtPoint2(u0, v0), dtPoint2(u1, v1));
        delete aGP;
        aGP = m2d;
      }
//      else {
//        dt__THROW(apply(),
//                << "Either _createSplineCurve3d or _createAnalyticSurface "
//                << "should be true.");
//      }
      aGeoRet.push_back( aGP );
    }
    
    return aGeoRet;
  }

  bool pickLengthRange::isNecessary( void ) const {
    return true;
  }
  
  void pickLengthRange::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
    _createSplineCurve3d = true;
    _createAnalyticSurface = false;

    if ( dtXmlParserBase::hasAttribute("length_one_start", *tE)
            && dtXmlParserBase::hasAttribute("length_two_start", *tE)
            && dtXmlParserBase::hasAttribute("length_one_end", *tE)
            && dtXmlParserBase::hasAttribute("length_two_end", *tE) ) {
      if (dtXmlParserBase::hasAttribute("length_one_start", *tE)) {
        _lengthOneStart = dtXmlParserBase::muParseString( dtXmlParserBase::replaceDependencies(
                                      dtXmlParserBase::getAttributeStr(
                                        "length_one_start",
                                        *tE
                                      ),
                                      cV, 
                                      aF) 
                                    );
      }

      if (dtXmlParserBase::hasAttribute("length_two_start", *tE)) {
        _lengthTwoStart = dtXmlParserBase::muParseString( dtXmlParserBase::replaceDependencies(
                                      dtXmlParserBase::getAttributeStr(
                                        "length_two_start",
                                        *tE
                                      ),
                                      cV, 
                                      aF) 
                                    );
      }

      if (dtXmlParserBase::hasAttribute("length_one_end", *tE)) {
        _lengthOneEnd = dtXmlParserBase::muParseString( dtXmlParserBase::replaceDependencies(
                                      dtXmlParserBase::getAttributeStr(
                                        "length_one_end",
                                        *tE
                                      ),
                                      cV, 
                                      aF) 
                                    );
      }

      if (dtXmlParserBase::hasAttribute("length_two_end", *tE)) {
        _lengthTwoEnd = dtXmlParserBase::muParseString( dtXmlParserBase::replaceDependencies(
                                      dtXmlParserBase::getAttributeStr(
                                        "length_two_end",
                                        *tE
                                      ),
                                      cV, 
                                      aF) 
                                    );
      }    

      //
      // input check
      //
      if ( (_lengthOneStart == _lengthOneEnd)
              && (_lengthTwoStart == _lengthTwoEnd) ) {
        dt__throw(init(),
                << dt__eval(_lengthOneStart) << std::endl
                << dt__eval(_lengthTwoStart) << std::endl
                << dt__eval(_lengthOneEnd) << std::endl
                << dt__eval(_lengthTwoEnd) << std::endl
                << "Either lengthOneStart and lengthOneEnd or/and" << std::endl
                << " lengthTwoStart and lengthTwoEnd should be different." << std::endl
                << " Both ranges equal means picking a point.");
      }
      if ( (_lengthOneStart != _lengthOneEnd)
              && (_lengthTwoStart != _lengthTwoEnd) ) {
        _createSplineCurve3d = false;
        _createAnalyticSurface = true;        
//        dt__THROW(init(),
//                << dt__eval(_lengthOneStart) << std::endl
//                << dt__eval(_lengthTwoStart) << std::endl
//                << dt__eval(_lengthOneEnd) << std::endl
//                << dt__eval(_lengthTwoEnd) << std::endl
//                << "Either lengthOneStart and lengthOneEnd or/and" << std::endl
//                << " lengthTwoStart and lengthTwoEnd are equal." << std::endl
//                << " Please use a scaFunction to pick this kind of curve.");
      }      
      if ( (_lengthOneStart > _lengthOneEnd)
              || (_lengthTwoStart > _lengthTwoEnd) ) {
        dt__throw(init(),
                << dt__eval(_lengthOneStart) << std::endl
                << dt__eval(_lengthTwoStart) << std::endl
                << dt__eval(_lengthOneEnd) << std::endl
                << dt__eval(_lengthTwoEnd) << std::endl
                << "Either lengthOneStart is bigger than lengthOneEnd or/and" << std::endl
                << " lengthTwoStart is bigger than lengthTwoEnd." << std::endl
                << " In both ranges start should be smaller than end.");
      }    
    }
    else {
      dt__throw(init(), << "Error in input values.");
    }
  }  
}
