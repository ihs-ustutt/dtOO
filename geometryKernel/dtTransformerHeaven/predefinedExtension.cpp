#include "predefinedExtension.h"
#include "geometryEngine/dtCurve.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
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
	
	predefinedExtension::predefinedExtension( 
	  predefinedExtension const & orig 
	) : dtTransformer(orig) {
		_yIn = orig._yIn;
		_yOut = orig._yOut;
		_alphaIn = orig._alphaIn;
		_alphaOut = orig._alphaOut;
	}

  predefinedExtension::~predefinedExtension() {
  }

  dtTransformer * predefinedExtension::clone( void ) const {
	  return new predefinedExtension(*this);
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

    for (int ii=0; ii<sFunP->size(); ii++) {
      //
      // check type
      //
			vec2dCurve2dOneD const * vS2 = vec2dCurve2dOneD::ConstDownCast(sFunP->at(ii));
			vec3dCurveOneD const * vS3 = vec3dCurveOneD::ConstDownCast(sFunP->at(ii));
			
			//
			// two dimensional curve
			//
			if (vS2) {
				dtCurve2d const * dtC2d = vS2->ptrDtCurve2d();

				//
				// get points, slope and distances
				//
				int nCPoints = dtC2d->nControlPoints();
				dtPoint2 start = dtC2d->controlPoint(0);
				dtPoint2 end = dtC2d->controlPoint(nCPoints-1);
				dtVector2 vvStart = start - dtC2d->controlPoint(1);
				vvStart = vvStart / sqrt(vvStart.squared_length());
				dtVector2 vvEnd = end - dtC2d->controlPoint(nCPoints-2);
				vvEnd = vvEnd / sqrt(vvEnd.squared_length());

				std::vector< dtPoint2 > pV 
				= 
				calculateExtPoints(start, vvStart, end, vvEnd);
				
				//
				// calculate points
				//
				std::vector< dtPoint2 > pVStart(3);
				pVStart[0] = pV[0];
				pVStart[1] = pV[1];
				pVStart[2] = pV[2];
				std::vector< dtPoint2 > pVEnd(3);
				pVEnd[0] = pV[3];
				pVEnd[1] = pV[4];
				pVEnd[2] = pV[5];
				
				//
				// create curve vector
				//
				vectorHandling< dtCurve2d const * > dtCC2d;
				dtCC2d.push_back( bSplineCurve2d_pointConstructOCC(pVStart, 2).result() );
				dtCC2d.push_back( dtC2d->clone() );
				dtCC2d.push_back( bSplineCurve2d_pointConstructOCC(pVEnd, 2).result() );

				//
				// connect curves
				//
				ptrHandling< dtCurve2d > newDtC2d(
					bSplineCurve2d_curveConnectConstructOCC(dtCC2d).result()
				);
				//
				// free memory
				//
				dtCC2d.destroy();

				//
				// create new curve
				//
				transSFun.push_back( new vec2dCurve2dOneD( newDtC2d.get() ) );
				transSFun.back()->setLabel( vS2->getLabel() );				
			}
			else if (vS3) {
				dtCurve const * dtC = vS3->ptrDtCurve();

				//
				// get points, slope and distances
				//
				int nCPoints = dtC->nControlPoints();
				dtPoint3 start = dtC->controlPoint(0);
				dtPoint3 end = dtC->controlPoint(nCPoints-1);
				dtVector3 vvStart = start - dtC->controlPoint(1);
				vvStart = vvStart / sqrt(vvStart.squared_length());
				dtVector3 vvEnd = end - dtC->controlPoint(nCPoints-2);
				vvEnd = vvEnd / sqrt(vvEnd.squared_length());

				std::vector< dtPoint2 > pV 
				= 
				calculateExtPoints(
					dtPoint2(start.x(), start.y()), dtVector2(vvStart.x(), vvStart.y()),
					dtPoint2(end.x(), end.y()), dtVector2(vvEnd.x(), vvEnd.y())
				);
				
				//
				// calculate points
				//
				std::vector< dtPoint3 > pVStart;
				pVStart.push_back(dtPoint3(pV[0].x(), pV[0].y(), start.z()));
				pVStart.push_back(dtPoint3(pV[1].x(), pV[1].y(), start.z()));
				pVStart.push_back(dtPoint3(pV[2].x(), pV[2].y(), start.z()));
				std::vector< dtPoint3 > pVEnd;
				pVEnd.push_back(dtPoint3(pV[3].x(), pV[3].y(), end.z()));
				pVEnd.push_back(dtPoint3(pV[4].x(), pV[4].y(), end.z()));
				pVEnd.push_back(dtPoint3(pV[5].x(), pV[5].y(), end.z()));

				//
				// create curve vector
				//
				vectorHandling< dtCurve const * > dtCC;
				dtCC.push_back( bSplineCurve_pointConstructOCC(pVStart, 2).result() );
				dtCC.push_back( dtC->clone() );
				dtCC.push_back( bSplineCurve_pointConstructOCC(pVEnd, 2).result() );

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
				dt__throw(apply(),
								<< dt__eval(vS2) << std::endl 
								<< dt__eval(vS3) );
			}
    }
    return transSFun;  
  }
		
  void predefinedExtension::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
    //
    // get input
    //
		_alphaIn = dtXmlParserBase::muParseString( 
								 dtXmlParserBase::replaceUsedFunctions(
									 dtXmlParserBase::getAttributeStr("alpha_in", *tE),
									 cV, 
						       aF
								 ) 
							 );
		_yIn = dtXmlParserBase::muParseString( 
						 dtXmlParserBase::replaceUsedFunctions(
						   dtXmlParserBase::getAttributeStr("y_in", *tE), cV, aF
						 ) 
					 );
		_alphaOut = dtXmlParserBase::muParseString( 
									dtXmlParserBase::replaceUsedFunctions(
										dtXmlParserBase::getAttributeStr("alpha_out", *tE),
										cV, 
						aF
									) 
								);
		_yOut = dtXmlParserBase::muParseString( 
							dtXmlParserBase::replaceUsedFunctions(
								dtXmlParserBase::getAttributeStr("y_out", *tE), cV, aF
							) 
						);     
  }

  bool predefinedExtension::isNecessary( void ) const {
    return true;
  }
	
	std::vector<dtPoint2> predefinedExtension::calculateExtPoints(
	  dtPoint2 const & start, dtVector2 const & vvStart,
		dtPoint2 const & end, dtVector2 const & vvEnd
	) const {
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
				std::vector< dtPoint2 > pVExt(6);
				pVExt[0] = start2;
				pVExt[1] = start1;
				pVExt[2] = start;
				pVExt[3] = end;
				pVExt[4] = end1;
				pVExt[5] = end2;

        return pVExt;
	}

}

