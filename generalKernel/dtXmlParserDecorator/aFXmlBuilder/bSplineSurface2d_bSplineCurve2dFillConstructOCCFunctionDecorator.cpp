#include "bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtSurface2d.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec2dSurface2dTwoD.h>
#include <geometryEngine/geoBuilder/bSplineSurface2d_bSplineCurve2dFillConstructOCC.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator::bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator() {
  }

  bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator::~bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator() {

  }

  void bSplineSurface2d_bSplineCurve2dFillConstructOCCFunctionDecorator::buildPart(
		QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
    //
    //check input
    //
    bool hasFunction = hasChild("analyticFunction", toBuildP);
    
    if ( hasFunction ) {
			vectorHandling< dtCurve2d const * > cc;
      std::vector< QDomElement > elVec 
			= 
			getChildVector("analyticFunction", toBuildP);
      dt__forAllIndex(elVec, ii) {
        dt__pH(analyticFunction) aF(
				  createAnalyticFunction(&elVec[ii], bC, cValP, depSFunP)
				);
				dt__ptrAss(
					vec2dCurve2dOneD const * v2d1d,
					vec2dCurve2dOneD::ConstDownCast(aF.get())
				);
				cc.push_back( v2d1d->ptrDtCurve2d()->clone() );
      }

			ptrHandling<dtSurface2d> dtS2d( 
				bSplineSurface2d_bSplineCurve2dFillConstructOCC(cc).result()
			);

      //
      // create scaCurve2dOneD
      //
      sFunP->push_back( new vec2dSurface2dTwoD( dtS2d.get() ) );			
    }
    else {
      dt__throw(buildPart(), << dt__eval(hasFunction) );
    }
  }
}
