#include "bSplineSurface_bSplineCurveFillConstructOCC.h"
#include "interfaceHeaven/staticPropertiesHandler.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/geoBuilder/bSplineCurve_convertOCC.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Precision.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomFill_FillingStyle.hxx>
#include <GeomFill_BSplineCurves.hxx>

namespace dtOO {
	bSplineSurface_bSplineCurveFillConstructOCC
    ::bSplineSurface_bSplineCurveFillConstructOCC( 
	  vectorHandling< dtCurve const * > const & cc 
	) {
		dt__throwIfWithMessage(
			cc.size() != 4,
			bSplineSurface_bSplineCurveFillConstructOCC(),
			<< "Only supported with 4 curves." << std::endl
			<< dt__eval(cc.size())
		);
		std::vector< Handle(Geom_BSplineCurve) > CC(4);
	  
		dt__pVH(dtOCCBSplineCurve) occC;
    dt__forAllIndex(CC, ii) {	    
      if ( dtOCCBSplineCurve::ConstDownCast(cc[ii]) ) {
        occC.push_back( dtOCCBSplineCurve::DownCast(cc[ii]->clone()) );
      }
      else {
        dt__warning(
          bSplineSurface_bSplineCurveFillConstructOCC(),
          << "Position [" << ii << "] is not a BSpline. Try to convert."
        );
        occC.push_back(
          dtOCCBSplineCurve::MustDownCast(
            bSplineCurve_convertOCC( *(cc[ii]) ).result() 
          )
        );
        
      }
		  CC[ii] = Handle(Geom_BSplineCurve)::DownCast( occC[ii].OCCRef().getOCC() );
    }

		GeomFill_BSplineCurves fill;
		dtOCCSurfaceBase base;	
		dt__tryOcc(
			fill.Init(
			  CC[0], CC[1], CC[2], CC[3], 
				GeomFill_FillingStyle::GeomFill_StretchStyle
			);						
			base.setOCC( fill.Surface() );
		,
			<< "Could not initialize and create filled surface." << std::endl
      << logMe::dtFormat("[0] (%e %e %e) -> (%e %e %e)\n")
        % cc[0]->pointPercent(0.).x() % cc[0]->pointPercent(0.).y() 
        % cc[0]->pointPercent(0.).z() % cc[0]->pointPercent(1.).x()
        % cc[0]->pointPercent(1.).y() % cc[0]->pointPercent(1.).z()
      << logMe::dtFormat("[1] (%e %e %e) -> (%e %e %e)\n")
        % cc[1]->pointPercent(0.).x() % cc[1]->pointPercent(0.).y()
        % cc[1]->pointPercent(0.).z() % cc[1]->pointPercent(1.).x()
        % cc[1]->pointPercent(1.).y() % cc[1]->pointPercent(1.).z()
       << logMe::dtFormat("[2] (%e %e %e) -> (%e %e %e)\n")
        % cc[2]->pointPercent(0.).x() % cc[2]->pointPercent(0.).y()
        % cc[2]->pointPercent(0.).z() % cc[2]->pointPercent(1.).x()
        % cc[2]->pointPercent(1.).y() % cc[2]->pointPercent(1.).z()          
       << logMe::dtFormat("[3] (%e %e %e) -> (%e %e %e)\n")
        % cc[3]->pointPercent(0.).x() % cc[3]->pointPercent(0.).y()
        % cc[3]->pointPercent(0.).z() % cc[3]->pointPercent(1.).x()
        % cc[3]->pointPercent(1.).y() % cc[3]->pointPercent(1.).z()  
		);
		_dtS.reset( new dtOCCBSplineSurface(base) );
	}

	bSplineSurface_bSplineCurveFillConstructOCC
    ::bSplineSurface_bSplineCurveFillConstructOCC( 
	  dt__pVH(dtCurve) const & cc 
	) {
	  vectorHandling< dtCurve const * > ccV;
		for (
			dt__pVH(dtCurve)::const_iterator it = cc.begin();
			it!=cc.end();
			++it
		) {
			ccV.push_back( &(*it) );
		}
		
		_dtS.reset( bSplineSurface_bSplineCurveFillConstructOCC(ccV).result() );
	}

  bSplineSurface_bSplineCurveFillConstructOCC
    ::bSplineSurface_bSplineCurveFillConstructOCC( 
      dtCurve const * const c0, dtCurve const * const c1, 
      dtCurve const * const c2, dtCurve const * const c3
    ) {
    vectorHandling< dtCurve const * > ccV(4, NULL);
    ccV[0] = c0;
    ccV[1] = c1;
    ccV[2] = c2;
    ccV[3] = c3;
    _dtS.reset( 
      bSplineSurface_bSplineCurveFillConstructOCC(ccV).result() 
    );
  }  
	
	bSplineSurface_bSplineCurveFillConstructOCC
    ::~bSplineSurface_bSplineCurveFillConstructOCC() {
	}
	
	dtSurface * bSplineSurface_bSplineCurveFillConstructOCC::result( void ) {
		return _dtS->clone();
	}
}

