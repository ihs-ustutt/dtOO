#include "pickLengthRange.h"
#include "geometryEngine/dtCurve.h"
#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>
#include <geometryEngine/dtSurface.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/vec2dOneD.h>

namespace dtOO {
  pickLengthRange::pickLengthRange() : dtTransformer() {
  }

  pickLengthRange::pickLengthRange(const pickLengthRange& orig) : dtTransformer(orig) {
  }

  pickLengthRange::~pickLengthRange() {
  }

  dtTransformer * pickLengthRange::clone( void ) const {
	  dt__THROW(clone(), "Not yet implemented.");
	}
	
  dtTransformer * pickLengthRange::create( void ) const {
		return new pickLengthRange();
	}
	
  vectorHandling< analyticGeometry * > pickLengthRange::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
    vectorHandling< analyticGeometry * > aGeoRet;
    
    for (int ii=0;ii<aGeoVecP->size();ii++) {
      analyticGeometry * aGP = aGeoVecP->at(ii)->clone();
      //
      // cast
      //
      analyticSurface * aSP;
      dt__MUSTDOWNCAST(aGP, analyticSurface, aSP);

      //
      if ( _createSplineCurve3d ) {
				dt__THROW(apply(), << "Not yet supported.");
      }
      else if (_createAnalyticSurface) {
				dt__pH(map1dTo3d) m1dU(aSP->segmentConstVPercent(0., 0., 1.));
				dt__PTRASS(splineCurve3d * s3dU, splineCurve3d::DownCast(m1dU.get()));
				float u0 = s3dU->ptrConstDtCurve()->uPercent_l(_lengthOneStart);
				float u1 = s3dU->ptrConstDtCurve()->uPercent_l(_lengthOneEnd);
				dt__pH(map1dTo3d) m1dV(aSP->segmentConstUPercent(0., 0., 1.));
				dt__PTRASS(splineCurve3d * s3dV, splineCurve3d::DownCast(m1dV.get()));
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
	  QDomElement const * transformerElementP, 
    baseContainer const * const bC,   
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP 
	) {
    dtTransformer::init(transformerElementP, bC, cValP, sFunP, depAGeoP);
		
    _createSplineCurve3d = true;
    _createAnalyticSurface = false;

    if ( hasAttribute("length_one_start", *transformerElementP)
            && hasAttribute("length_two_start", *transformerElementP)
            && hasAttribute("length_one_end", *transformerElementP)
            && hasAttribute("length_two_end", *transformerElementP) ) {
      if (transformerElementP->hasAttribute("length_one_start")) {
        _lengthOneStart = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "length_one_start",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }

      if (transformerElementP->hasAttribute("length_two_start")) {
        _lengthTwoStart = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "length_two_start",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }

      if (transformerElementP->hasAttribute("length_one_end")) {
        _lengthOneEnd = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "length_one_end",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }

      if (transformerElementP->hasAttribute("length_two_end")) {
        _lengthTwoEnd = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "length_two_end",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }    

      //
      // input check
      //
      if ( (_lengthOneStart == _lengthOneEnd)
              && (_lengthTwoStart == _lengthTwoEnd) ) {
        dt__THROW(init(),
                << DTLOGEVAL(_lengthOneStart) << LOGDEL
                << DTLOGEVAL(_lengthTwoStart) << LOGDEL
                << DTLOGEVAL(_lengthOneEnd) << LOGDEL
                << DTLOGEVAL(_lengthTwoEnd) << LOGDEL
                << "Either lengthOneStart and lengthOneEnd or/and" << LOGDEL
                << " lengthTwoStart and lengthTwoEnd should be different." << LOGDEL
                << " Both ranges equal means picking a point.");
      }
      if ( (_lengthOneStart != _lengthOneEnd)
              && (_lengthTwoStart != _lengthTwoEnd) ) {
        _createSplineCurve3d = false;
        _createAnalyticSurface = true;        
//        dt__THROW(init(),
//                << DTLOGEVAL(_lengthOneStart) << LOGDEL
//                << DTLOGEVAL(_lengthTwoStart) << LOGDEL
//                << DTLOGEVAL(_lengthOneEnd) << LOGDEL
//                << DTLOGEVAL(_lengthTwoEnd) << LOGDEL
//                << "Either lengthOneStart and lengthOneEnd or/and" << LOGDEL
//                << " lengthTwoStart and lengthTwoEnd are equal." << LOGDEL
//                << " Please use a scaFunction to pick this kind of curve.");
      }      
      if ( (_lengthOneStart > _lengthOneEnd)
              || (_lengthTwoStart > _lengthTwoEnd) ) {
        dt__THROW(init(),
                << DTLOGEVAL(_lengthOneStart) << LOGDEL
                << DTLOGEVAL(_lengthTwoStart) << LOGDEL
                << DTLOGEVAL(_lengthOneEnd) << LOGDEL
                << DTLOGEVAL(_lengthTwoEnd) << LOGDEL
                << "Either lengthOneStart is bigger than lengthOneEnd or/and" << LOGDEL
                << " lengthTwoStart is bigger than lengthTwoEnd." << LOGDEL
                << " In both ranges start should be smaller than end.");
      }    
    }
    else {
      dt__THROW(init(), << "Error in input values.");
    }
  }  
}
