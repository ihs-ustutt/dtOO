#include "predefinedExtension.h"
#include "geometryEngine/dtCurve.h"
#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_curveConnectConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <dtLinearAlgebra.h>

#include <progHelper.h>
#include <logMe/logMe.h>

namespace dtOO {
  predefinedExtension::predefinedExtension() : dtTransformer() {
  }

  predefinedExtension::~predefinedExtension() {
  }

  dtTransformer * predefinedExtension::clone( void ) const {
	  dt__THROW(clone(), "Not yet implemented.");
	}
	
  dtTransformer * predefinedExtension::create( void ) const {
		return new predefinedExtension();
	}
	
  /**
   * 
   * @todo What if first and last control points are duplicate?
   */
  vectorHandling< analyticFunction * > predefinedExtension::apply( vectorHandling< analyticFunction * > const * const sFunP ) const {
    vectorHandling< analyticFunction * > transSFun;

    for (int ii=0;ii<sFunP->size();ii++) {
      //
      // check type
      //
			vec2dCurve2dOneD const * vS2 = vec2dCurve2dOneD::ConstDownCast(sFunP->at(ii));
			vec3dCurveOneD const * vS3 = vec3dCurveOneD::ConstDownCast(sFunP->at(ii));
			
			//
			// two dimensional curve
			//
			if (vS2) {
				dtCurve2d const * sS2DtC = vS2->ptrDtCurve2d();

				int order = sS2DtC->order();

				//
				// get points, slope and distances
				//
				int nCPoints = sS2DtC->nControlPoints();
				dtPoint2 start = sS2DtC->controlPoint(0);
				dtPoint2 end = sS2DtC->controlPoint(nCPoints-1);
				dtVector2 vvStart = start - sS2DtC->controlPoint(1);
				vvStart = vvStart / sqrt(vvStart.squared_length());
				dtVector2 vvEnd = end - sS2DtC->controlPoint(nCPoints-2);
				vvEnd = vvEnd / sqrt(vvEnd.squared_length());
	//      dtVector2 vvY = dtVector2(0,1);

				//
				// calculate new points
				//
				float xx = _alphaIn * (_yIn - start.y())*vvStart.x()/vvStart.y() + start.x();
				dtPoint2 start1( xx, start.y() + _alphaIn * (_yIn-start.y()) );
				dtPoint2 start2( xx, _yIn);

				xx = end.x() + vvEnd.x() * _alphaOut * (_yOut - end.y())/vvEnd.y();
				dtPoint2 end1( xx, end.y() + _alphaOut * (_yOut - end.y()));
				dtPoint2 end2( xx, _yOut);

				//
				// calculate points
				//
				std::vector< dtPoint2 > pVStart;
				pVStart.push_back(start2);
				pVStart.push_back(start1);
				pVStart.push_back(start);
				std::vector< dtPoint2 > pVEnd;
				pVEnd.push_back(end);
				pVEnd.push_back(end1);
				pVEnd.push_back(end2);

				//
				// create curve vector
				//
				vectorHandling< dtCurve2d const * > dtC2d;
				dtC2d.push_back( bSplineCurve2d_pointConstructOCC(pVStart, 1).result() );
				dtC2d.push_back( sS2DtC->clone() );
				dtC2d.push_back( bSplineCurve2d_pointConstructOCC(pVEnd, 1).result() );

				//
				// connect curves
				//
				ptrHandling< dtCurve2d > newDtC2(
					bSplineCurve2d_curveConnectConstructOCC(dtC2d).result()
				);
				//
				// free memory
				//
				dtC2d.destroy();

				//
				// create new curve
				//
				transSFun.push_back( new vec2dCurve2dOneD( newDtC2.get() ) );
				transSFun.back()->setLabel( vS2->getLabel() );
			}
			else if (vS3) {
				dtCurve const * dtC = vS3->ptrDtCurve();

				//
				// get points, slope and distances
				//
				int nCPoints = dtC->getNControlPoints();
				dtPoint3 start = dtC->getControlPoint3d(0);
				dtPoint3 end = dtC->getControlPoint3d(nCPoints-1);
				dtVector3 vvStart = start - dtC->getControlPoint3d(1);
				vvStart = vvStart / sqrt(vvStart.squared_length());
				dtVector3 vvEnd = end - dtC->getControlPoint3d(nCPoints-2);
				vvEnd = vvEnd / sqrt(vvEnd.squared_length());

				//
				// calculate new points
				//			
				dtPoint3 start1 = start;
				dtPoint3 start2 = start - (_yIn - start.y())*vvStart;
				dtPoint3 end1 = end;
				dtPoint3 end2 = end + (_yOut - end.y())*vvEnd;
				
				//
				// calculate points
				//
				std::vector< dtPoint3 > pVStart;
				pVStart.push_back(start2);
				pVStart.push_back(start1);
				std::vector< dtPoint3 > pVEnd;
				pVEnd.push_back(end1);
				pVEnd.push_back(end2);

				//
				// create curve vector
				//
				vectorHandling< dtCurve const * > dtCC;
				dtCC.push_back( bSplineCurve_pointConstructOCC(pVStart, 1).result() );
				dtCC.push_back( dtC->clone() );
				dtCC.push_back( bSplineCurve_pointConstructOCC(pVEnd, 1).result() );

				//
				// connect curves
				//
				ptrHandling< dtCurve > newDtC(
					bSplineCurve_curveConnectConstructOCC(dtCC).result()
				);
				//
				// free memory
				//
				dtCC.destroy();

				//
				// create new curve
				//
				transSFun.push_back( new vec3dCurveOneD( newDtC.get() ) );
				transSFun.back()->setLabel( vS3->getLabel() );
			}			
			else {
				dt__THROW(apply(),
								<< DTLOGEVAL(vS2) << LOGDEL 
								<< DTLOGEVAL(vS3) );
			}
    }
    return transSFun;  
  }

  void predefinedExtension::init( QDomElement const * transformerElementP, 
                                  vectorHandling< constValue * > const * const cValP,
                                  vectorHandling< analyticFunction * > const * const sFunP ) {
    dtTransformer::init(transformerElementP, cValP, sFunP);
    //
    // check input
    //
    bool hasAlphaIn = transformerElementP->hasAttribute("alpha_in");
    bool hasYIn = transformerElementP->hasAttribute("y_in");
    bool hasAlphaOut = transformerElementP->hasAttribute("alpha_out");
    bool hasYOut = transformerElementP->hasAttribute("y_out");
    
    //
    // get input
    //
    if (hasAlphaIn) {    
      _alphaIn = muParseString( 
                   replaceUsedFunctions(
                     getAttributeStr(
                       "alpha_in", 
                       *transformerElementP
                     ),
                     cValP, 
                     sFunP
                   ) 
                 );
		}
		if (hasYIn) {    
      _yIn = muParseString( 
                  replaceUsedFunctions(
                    getAttributeStr(
                      "y_in", 
                      *transformerElementP
                    ),
                    cValP, 
                    sFunP
                  ) 
                );
		}
		if (hasAlphaOut) {    
      _alphaOut = muParseString( 
                    replaceUsedFunctions(
                      getAttributeStr(
                        "alpha_out", 
                        *transformerElementP
                      ),
                      cValP, 
                      sFunP
                    ) 
                  );
		}
		if (hasYOut) {    
      _yOut = muParseString( 
                   replaceUsedFunctions(
                     getAttributeStr(
                       "y_out", 
                       *transformerElementP
                     ),
                     cValP, 
                     sFunP
                   ) 
                 );     
		}
//    }
//    else {
      DTINFOWF(
        init(),
        << DTLOGEVAL(hasAlphaIn) << LOGDEL
        << DTLOGEVAL(hasAlphaOut) << LOGDEL
        << DTLOGEVAL(hasYIn) << LOGDEL
        << DTLOGEVAL(hasYOut) << LOGDEL
				<< DTLOGEVAL(_alphaIn) << LOGDEL
				<< DTLOGEVAL(_alphaOut) << LOGDEL
				<< DTLOGEVAL(_yIn) << LOGDEL
				<< DTLOGEVAL(_yOut)
      );       
//    }
  }

  bool predefinedExtension::isNecessary( void ) const {
    return true;
  }
}

