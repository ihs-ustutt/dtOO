#include "pickRange.h"
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
  pickRange::pickRange() {
  }

  pickRange::pickRange(const pickRange& orig) {
  }

  pickRange::~pickRange() {
  }

  vectorHandling< analyticGeometry * > pickRange::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
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
        if ( _parameterPercentFunction == NULL ) {
          //
          // pick const u direction
          //
          if (_parameterOneStartPercent == _parameterOneEndPercent) {               
            //
            // pick curve
            //
            map1dTo3d * m1d = aSP->pickConstUPercent(
                                         _parameterOneStartPercent,
                                         _parameterTwoStartPercent,
                                         _parameterTwoEndPercent
                                       );

            //
            // delete analyticGeometry and set pointer to splineCurve3d
            //
            delete aGP;                
            aGP = m1d;
          }
          //
          // pick const v direction
          //
          else if (_parameterTwoStartPercent == _parameterTwoEndPercent) {
            //
            // pick curve
            //
            map1dTo3d * m1d = aSP->pickConstUPercent(
                                         _parameterTwoStartPercent,
                                         _parameterOneStartPercent,
                                         _parameterOneEndPercent
                                       );

            //
            // delete analyticGeometry and set pointer to splineCurve3d
            //
            delete aGP;                
            aGP = m1d;           
          }
        }
        else {
          //
          // init new spline and set curve to it
          //
          vec2dOneDInMap2dTo3d * v2dInM2dTo3d 
					= 
					new vec2dOneDInMap2dTo3d( _parameterPercentFunction, aSP, true);                

          //
          // delete analyticGeometry and set pointer to splineCurve3d
          //
          delete aGP;                
          aGP = v2dInM2dTo3d;                  
        }
      }
      else if (_createAnalyticSurface) {
        map2dTo3d * m2d
				=
				aSP->pickPercent(
				  _parameterOneStartPercent,
					_parameterOneEndPercent,
					_parameterTwoStartPercent,
					_parameterTwoEndPercent
				);				
        delete aGP;
        aGP = m2d;
      }
      else {
        dt__THROW(apply(),
                << "Either _createSplineCurve3d or _createAnalyticSurface "
                << "should be true.");
      }
      aGeoRet.push_back( aGP );
    }
    
    return aGeoRet;
  }

  bool pickRange::isNecessary( void ) const {
    return true;
  }
  
  void pickRange::init( QDomElement * transformerElementP, 
                     pointContainer * const pointContainerP,
                     vectorContainer * const vectorContainerP,    
                     vectorHandling< constValue * > const * const cValP,
                     vectorHandling< analyticFunction * > const * const sFunP,
                     vectorHandling< analyticGeometry * > const * const depAGeoP ) {
    
    _parameterPercentFunction = NULL;
    _maxStep = -1.;
    _tolerance = .01;
    _createSplineCurve3d = true;
    _createAnalyticSurface = false;

    if ( hasAttribute("parameter_one_start_percent", *transformerElementP)
            && hasAttribute("parameter_two_start_percent", *transformerElementP)
            && hasAttribute("parameter_one_end_percent", *transformerElementP)
            && hasAttribute("parameter_two_end_percent", *transformerElementP) ) {
      if (transformerElementP->hasAttribute("parameter_one_start_percent")) {
        _parameterOneStartPercent = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "parameter_one_start_percent",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }

      if (transformerElementP->hasAttribute("parameter_two_start_percent")) {
        _parameterTwoStartPercent = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "parameter_two_start_percent",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }

      if (transformerElementP->hasAttribute("parameter_one_end_percent")) {
        _parameterOneEndPercent = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "parameter_one_end_percent",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }

      if (transformerElementP->hasAttribute("parameter_two_end_percent")) {
        _parameterTwoEndPercent = muParseString( replaceUsedFunctions(
                                      getAttributeStr(
                                        "parameter_two_end_percent",
                                        *transformerElementP
                                      ),
                                      cValP, 
                                      sFunP) 
                                    );
      }    

      //
      // input check
      //
      if ( (_parameterOneStartPercent == _parameterOneEndPercent)
              && (_parameterTwoStartPercent == _parameterTwoEndPercent) ) {
        dt__THROW(init(),
                << DTLOGEVAL(_parameterOneStartPercent) << LOGDEL
                << DTLOGEVAL(_parameterTwoStartPercent) << LOGDEL
                << DTLOGEVAL(_parameterOneEndPercent) << LOGDEL
                << DTLOGEVAL(_parameterTwoEndPercent) << LOGDEL
                << "Either parameterOneStartPercent and parameterOneEndPercent or/and" << LOGDEL
                << " parameterTwoStartPercent and parameterTwoEndPercent should be different." << LOGDEL
                << " Both ranges equal means picking a point.");
      }
      if ( (_parameterOneStartPercent != _parameterOneEndPercent)
              && (_parameterTwoStartPercent != _parameterTwoEndPercent) ) {
        _createSplineCurve3d = false;
        _createAnalyticSurface = true;        
//        dt__THROW(init(),
//                << DTLOGEVAL(_parameterOneStartPercent) << LOGDEL
//                << DTLOGEVAL(_parameterTwoStartPercent) << LOGDEL
//                << DTLOGEVAL(_parameterOneEndPercent) << LOGDEL
//                << DTLOGEVAL(_parameterTwoEndPercent) << LOGDEL
//                << "Either parameterOneStartPercent and parameterOneEndPercent or/and" << LOGDEL
//                << " parameterTwoStartPercent and parameterTwoEndPercent are equal." << LOGDEL
//                << " Please use a scaFunction to pick this kind of curve.");
      }      
      if ( (_parameterOneStartPercent > _parameterOneEndPercent)
              || (_parameterTwoStartPercent > _parameterTwoEndPercent) ) {
        dt__THROW(init(),
                << DTLOGEVAL(_parameterOneStartPercent) << LOGDEL
                << DTLOGEVAL(_parameterTwoStartPercent) << LOGDEL
                << DTLOGEVAL(_parameterOneEndPercent) << LOGDEL
                << DTLOGEVAL(_parameterTwoEndPercent) << LOGDEL
                << "Either parameterOneStartPercent is bigger than parameterOneEndPercent or/and" << LOGDEL
                << " parameterTwoStartPercent is bigger than parameterTwoEndPercent." << LOGDEL
                << " In both ranges start should be smaller than end.");
      }    
    }
    else if( hasAttribute("parameter_function_percent", *transformerElementP) ) {
      if (transformerElementP->hasAttribute("parameter_function_percent")) {
        std::string funcName = getAttributeStr(
                                  "parameter_function_percent",
                                  *transformerElementP
                                );
        //
        // search functions
        //
				dt__PTRASS(
          vec2dOneD const * _parameterPercentFunction,								
				  vec2dOneD::ConstDownCast(sFunP->get(funcName))
				);
      }
      if (transformerElementP->hasAttribute("tolerance")) {
        _tolerance = muParseString( replaceUsedFunctions(
                         getAttributeStr("tolerance", *transformerElementP),
                         cValP, 
                         sFunP) 
                       );
      } 
      if (transformerElementP->hasAttribute("max_step")) {
        _maxStep = muParseString( replaceUsedFunctions(
                         getAttributeStr("max_step", *transformerElementP),
                         cValP, 
                         sFunP) 
                       );
      }           
    }
    else {
      dt__THROW(init(),
              << "Error in input values.");
    }
  }  
}
