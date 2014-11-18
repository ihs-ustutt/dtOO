#include "pickLengthPercentRange.h"
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
  pickLengthPercentRange::pickLengthPercentRange() : dtTransformer() {
  }

  pickLengthPercentRange::pickLengthPercentRange(const pickLengthPercentRange& orig) : dtTransformer(orig) {
  }

  pickLengthPercentRange::~pickLengthPercentRange() {
  }

  dtTransformer * pickLengthPercentRange::clone( void ) const {
	  dt__THROW(clone(), "Not yet implemented.");
	}
	
  dtTransformer * pickLengthPercentRange::create( void ) const {
		return new pickLengthPercentRange();
	}	

  vectorHandling< analyticGeometry * > pickLengthPercentRange::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
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
				dt__pH(map1dTo3d) m1dU(aSP->pickConstVPercent(0., 0., 1.));
				dt__PTRASS(splineCurve3d * s3dU, splineCurve3d::DownCast(m1dU.get()));
				float u0 = s3dU->ptrConstDtCurve()->uPercent_lPercent(_lengthPercentOneStart);
				float u1 = s3dU->ptrConstDtCurve()->uPercent_lPercent(_lengthPercentOneEnd);
				dt__pH(map1dTo3d) m1dV(aSP->pickConstUPercent(0., 0., 1.));
				dt__PTRASS(splineCurve3d * s3dV, splineCurve3d::DownCast(m1dV.get()));
				float v0 = s3dV->ptrConstDtCurve()->uPercent_lPercent(_lengthPercentTwoStart);
				float v1 = s3dV->ptrConstDtCurve()->uPercent_lPercent(_lengthPercentTwoEnd);
				
        map2dTo3d * m2d = aSP->pickPercent(u0, u1, v0, v1);				
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

  bool pickLengthPercentRange::isNecessary( void ) const {
    return true;
  }
  
  void pickLengthPercentRange::init( 
	  QDomElement * transformerElementP, 
    baseContainer * const bC,  
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP 
	) {
    _createSplineCurve3d = true;
    _createAnalyticSurface = false;

    if ( hasAttribute("length_percent_one_start", *transformerElementP)
            && hasAttribute("length_percent_two_start", *transformerElementP)
            && hasAttribute("length_percent_one_end", *transformerElementP)
            && hasAttribute("length_percent_two_end", *transformerElementP) ) {
      if (transformerElementP->hasAttribute("length_percent_one_start")) {
        _lengthPercentOneStart = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "length_percent_one_start",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }

      if (transformerElementP->hasAttribute("length_percent_two_start")) {
        _lengthPercentTwoStart = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "length_percent_two_start",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }

      if (transformerElementP->hasAttribute("length_percent_one_end")) {
        _lengthPercentOneEnd = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "length_percent_one_end",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }

      if (transformerElementP->hasAttribute("length_percent_two_end")) {
        _lengthPercentTwoEnd = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "length_percent_two_end",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }    

      //
      // input check
      //
      if ( (_lengthPercentOneStart == _lengthPercentOneEnd)
              && (_lengthPercentTwoStart == _lengthPercentTwoEnd) ) {
        dt__THROW(init(),
                << DTLOGEVAL(_lengthPercentOneStart) << LOGDEL
                << DTLOGEVAL(_lengthPercentTwoStart) << LOGDEL
                << DTLOGEVAL(_lengthPercentOneEnd) << LOGDEL
                << DTLOGEVAL(_lengthPercentTwoEnd) << LOGDEL
                << "Either lengthOneStart and lengthOneEnd or/and" << LOGDEL
                << " lengthTwoStart and lengthTwoEnd should be different." << LOGDEL
                << " Both ranges equal means picking a point.");
      }
      if ( (_lengthPercentOneStart != _lengthPercentOneEnd)
              && (_lengthPercentTwoStart != _lengthPercentTwoEnd) ) {
        _createSplineCurve3d = false;
        _createAnalyticSurface = true;        
//        dt__THROW(init(),
//                << DTLOGEVAL(_lengthPercentOneStart) << LOGDEL
//                << DTLOGEVAL(_lengthPercentTwoStart) << LOGDEL
//                << DTLOGEVAL(_lengthPercentOneEnd) << LOGDEL
//                << DTLOGEVAL(_lengthPercentTwoEnd) << LOGDEL
//                << "Either lengthOneStart and lengthOneEnd or/and" << LOGDEL
//                << " lengthTwoStart and lengthTwoEnd are equal." << LOGDEL
//                << " Please use a scaFunction to pick this kind of curve.");
      }      
      if ( (_lengthPercentOneStart > _lengthPercentOneEnd)
              || (_lengthPercentTwoStart > _lengthPercentTwoEnd) ) {
        dt__THROW(init(),
                << DTLOGEVAL(_lengthPercentOneStart) << LOGDEL
                << DTLOGEVAL(_lengthPercentTwoStart) << LOGDEL
                << DTLOGEVAL(_lengthPercentOneEnd) << LOGDEL
                << DTLOGEVAL(_lengthPercentTwoEnd) << LOGDEL
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
